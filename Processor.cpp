#include "Processor.hpp"
#include "MimeTypes.hpp"
#include "Utils.hpp"
#include "Config.hpp"
#include "Location.hpp"
#include "Logger.hpp"
#include "CgiProcess.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstring>
#include <cerrno>
#include <cstdio>

using namespace std;
Processor::Processor(MyServer &server) : serv(server)
{
}

Response Processor::processGET(Request &r, const Location &loc)
{
    Response res(serv);

    string path = buildPath(r);
    FileType ft = getFileType(path);
    if (ft == Directory)
    {
        if (path[path.length() - 1] != '/')
        {
            path += "/";
        }
        path += loc.getIndex()[0];
        ft = getFileType(path);
    }
    if (ft == DoesNotExist)
    {
        res.setStatus("404");
        return res;
    }
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1)
    {
        logger.log(LogType::DEBUG, strerror(errno));
        res.setStatus("503");
        return res;
    }
    struct stat st;
    fstat(fd, &st);
    char *buff = new char[st.st_size];
    read(fd, buff, st.st_size);
    close(fd);
    if (r.getMethod() == "GET")
        res.setBody(string(buff, st.st_size));
    res.setHeader("Last-Modified", timeToHTML(st.st_mtim.tv_sec));

    res.setStatus("200");
    delete[]buff;
    return res;
}

Response Processor::processPOST(Request &r, const Location &loc)
{
    size_t size = r.getBody().size();
    if( size > loc.getMaxBody())
    {
        Response res(serv);
        res.setStatus("413");
        return res;
    }
    r.setRoot(getLanguageRoot(r));
    string cgiName = loc.getCgiScript();
    string cgiPath = getLanguageRoot(r) + cgiName;
    CgiProcess cgi(serv, r, cgiPath, cgiName);
    Response res = cgi.processCGI();

    string methods = join(", ", loc.getMethods());
    res.setHeader("Allow", methods);
    //    res.setStatus("201");
    return res;
}

Response Processor::processPUT(Request &r, const Location &loc)
{
    Response res(serv);

    string path = buildPath(r);
    FileType ft = getFileType(path);
    if (ft == Directory)
    {
        res.setStatus("400");
        return res;
    }
    int fd;
    if (ft == DoesNotExist)
        fd = open(path.c_str(), O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    else
        fd = open(path.c_str(), O_TRUNC | O_RDWR);
    if (fd == -1)
    {
        logger.log(LogType::DEBUG, strerror(errno));
        res.setStatus("503");
        return res;
    }
    const char *body = r.getBody().c_str();
    size_t bodySize = r.getBody().size();
    int w = write(fd, body, bodySize);
    if (w == -1)
    {
        logger.log(LogType::DEBUG, strerror(errno));
        close(fd);
        res.setStatus("503");
        return res;
    }
    struct stat st;
    fstat(fd, &st);
    res.setHeader("Last-Modified", timeToHTML(st.st_mtim.tv_sec));
    res.setHeader("Content-Location", r.getUri().getPath());
    string methods = join(", ", loc.getMethods());
    res.setHeader("Allow", methods);
    setContType(res, path);
    close(fd);
    if (r.getBody().size() == 0)
        res.setStatus("204");
    else
        res.setStatus("200");
    return res;
}

Response Processor::processDELETE(Request &r, const Location &loc)
{
    Response res(serv);

    string path = buildPath(r);
    FileType ft = getFileType(path);
    if (ft == DoesNotExist)
    {
        res.setStatus("403");
        return res;
    }
    else if (ft == Directory)
    {
        res.setStatus("400");
        return res;
    }
    if (remove(path.c_str()) == -1)
    {
        logger.log(LogType::ERROR, strerror(errno));
        res.setStatus("304");
        return res;
    }
    string methods = join(", ", loc.getMethods());
    res.setHeader("Allow", methods);
    setContType(res, path);
    if (r.getBody().size() == 0)
        res.setStatus("204");
    else
        res.setStatus("200");
    return res;
}

string Processor::getLanguageRoot(Request &r)
{
    string path;
    auto iAcceptLang = r.getHeaders().find("accept-language");
    if (iAcceptLang != r.getHeaders().end())
    {
        for (const string &s : iAcceptLang->second)
        {
            auto iLang = serv.getConf().getLanguages().find(s);
            if (iLang != serv.getConf().getLanguages().end())
                return iLang->second;
        }
    }
    auto iDefLang = serv.getConf().getLanguages().find(serv.getConf().getDefLanguage());
    if (iDefLang != serv.getConf().getLanguages().end())
        return iDefLang->second;
    return serv.getConf().getRoot();
}

string Processor::buildPath(Request &r)
{
    string path = getLanguageRoot(r) + r.getUri().getPath();
    return path;
}

Response Processor::processRequest(Request &r)
{
    bool methodMatched = false;
    bool UriMatched = false;
    vector<string> errStatus;
    Response rsp(serv);
    if (r.getHttpVersion() != serv.getConf().getProtocol())
    {
        rsp.setStatus("505");
        return rsp;
    }

    for (const auto &l : serv.getConf().getLocations())
    {
        vector<string> matchedUri = l.matchesUri(r.getUri().getPath());
        if (matchedUri.empty())
            continue;
        for (const auto &path : matchedUri)
        {
            UriMatched = true; 
            r.setUri(RequestURI(path, r.getUri().getQuery(), r.getUri().getFragment()));

            bool test = l.isMatchMethod(r.getMethod());
            if (!test)
                continue;
            if (r.getMethod() == "GET" || r.getMethod() == "HEAD") // The GET method requests a representation of the specified resource. Requests using GET should only retrieve data.
            {
                methodMatched = true;
                Response t = processGET(r, l);
                if (t.getStatus()[0] == '2')
                    return t;
                else
                    errStatus.push_back(t.getStatus());
            }
            else if (r.getMethod() == "POST") //The POST method is used to submit an entity to the specified resource, often causing a change in state or side effects on the server.
            {
                methodMatched = true;
                Response t = processPOST(r, l);
                if (t.getStatus()[0] == '2')
                    return t;
                else
                    errStatus.push_back(t.getStatus());
            }
            else if (r.getMethod() == "PUT") // The PUT method replaces all current representations of the target resource with the request payload.
            {
                methodMatched = true;
                Response t = processPUT(r, l);
                if (t.getStatus()[0] == '2')
                    return t;
                else
                    errStatus.push_back(t.getStatus());
            }
            else if (r.getMethod() == "DELETE") 
            {
                methodMatched = true;
                Response t = processDELETE(r, l);
                if (t.getStatus()[0] == '2')
                    return t;
                else
                    errStatus.push_back(t.getStatus());
            }
        }
    }
    if (UriMatched && !methodMatched)
    {
        rsp.setStatus("405");
        return rsp;
    }
    if (!errStatus.empty())
        rsp.setStatus(errStatus.back());
    else
        rsp.setStatus("404");
    return rsp;
}

//         // else if (r.getMethod() == "CONNECT")
//         // {
//         //     // The CONNECT method establishes a tunnel to the server identified by the target resource.
//         // }
//         // else if (r.getMethod() == "OPTIONS")
//         // {
//         //     // The OPTIONS method is used to describe the communication options for the target resource.
//         // }
//         // else if (r.getMethod() == "TRACE")
//         // {
//         //     // The TRACE method performs a message loop-back test along the path to the target resource.
//         // }
//         // else if (r.getMethod() == "PATCH")
//         // {
//         //     // The PATCH method is used to apply partial modifications to a resource.
//         // }


void Processor::setContType(Response &res, const string &path)
{
    string ext = getFileExt(path);
    if (mimeTypes.find(ext) != mimeTypes.end())
    {
        string mimeType = mimeTypes[ext];
        res.setHeader("Content-Type", mimeType);
    }
    else
    {
        res.setHeader("Content-Type", "application/octet-stream");
    }
}