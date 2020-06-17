#include "CgiProcess.hpp"
#include "MyServer.hpp"
#include "Writer.hpp"
#include "Reader.hpp"
#include "Config.hpp"
#include "StringBuffer.hpp"
#include "StringReader.hpp"
#include "Logger.hpp"
#include <algorithm>
#include "Utils.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

using namespace std;

// AUTH_TYPE
// CONTENT_LENGTH *
// CONTENT_TYPE*
// GATEWAY_INTERFACE *
// PATH_INFO *
// PATH_TRANSLATED ?
// QUERY_STRING*
// REMOTE_ADDR*
// REMOTE_IDENT*
// REMOTE_USER *
// REQUEST_METHOD *
// REQUEST_URI *
// SCRIPT_NAME *
// SERVER_NAME *
// SERVER_PORT *
// SERVER_PROTOCOL *
// SERVER_SOFTWARE *

CgiProcess::CgiProcess(MyServer &serv, Request &req, const string &cgiPath, const string &cgiName)
:serv(serv), req(req)
{
    this->path = path;
    this->reqBody = req.getBody();
    this->cgiPath = cgiPath;
    this->cgiName = cgiName;
    env["DOCUMENT_ROOT"] = req.getRoot(); 
    env["REQUEST_METHOD"] = req.getMethod();
    env["SERVER_PROTOCOL"] = serv.getConf().getProtocol();
    env["SERVER_NAME"] = serv.getHostAddr();
    string bodySize = to_string(req.getBody().size());
    env["CONTENT_LENGTH"] = bodySize;
    string port = to_string(serv.getConf().getPort());
    env["SERVER_PORT"] = port;
    env["REQUEST_URI"] = req.getUri().getUriStr(); 
    env["SCRIPT_NAME"] = cgiName;
    env["SCRIPT_FILENAME"] = cgiPath;
    env["PATH_INFO"] = req.getUri().getUriStr();
    string pt = req.getRoot() + req.getUri().getUriStr();
    env["PATH_TRANSLATED"] = pt;
    env["QUERY_STRING"] = req.getUri().getQuery();
    auto find = req.getHeaders().find("content-type");
    env["CONTENT_TYPE"] = find->second[0];
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["SERVER_SOFTWARE"] = serv.getConf().getServName();
    env["REMOTE_ADDR"] = req.getInAddr();
    env["REMOTE_USER"] = "1";
    env["REMOTE_IDENT"] = "1";
    HTTPheadersToEnv();
}


void CgiProcess::HTTPheadersToEnv()
{
    for(auto i:req.getHeaders())
    {
        string name = "HTTP_" + convertStrToUpWithUdsc(i.first);
        env[name] = i.second[0];
    }

}

CgiProcess::~CgiProcess() {}

Response CgiProcess::processCGI()
{
    int fd1[2]; // Used to store two ends of first pipe
    int fd2[2];
    string response;
    if (pipe(fd1) == -1 || pipe(fd2) == -1)
    {
        logger.log(LogType::ERROR, string("creating pipe faled ") + strerror(errno));
    }
    pid_t pid = fork();

    if (pid == -1)
    {
        logger.log(LogType::ERROR, string("can not fork ") + strerror(errno));
    }

    if (pid == 0)
    {
        // Child process
        int count = 10;
        while(chdir(req.getRoot().c_str()) == -1)
        {
            count --;
            usleep(1);
        }
        if(!count)
            logger.log(LogType::ERROR, string("can't change directory ") + strerror(errno));
        serv.~MyServer();
        vector<const char *> envarg;
        vector<string> tmpenv;
        char *args[] = {0};
        for (auto &i : env)
        {
            tmpenv.push_back(i.first + "=" + i.second);
            logger.log(LogType::DEBUG, tmpenv.back());
        }
        for(const auto &i:tmpenv)
        {
            envarg.push_back(i.c_str());
        }
        envarg.push_back(nullptr);
        // char* const* p = const_cast<char* const*>(envarg.data());
        // while(*p)
        // {
        //     logger.log(LogType::DEBUG, string("p = ") + (*p));
        //     p++;
        // }
        //***************
        close(fd1[1]); // Close writing end of first pipe
        dup2(fd1[0], 0);

        close(fd2[0]);
        dup2(fd2[1], 1);
        //*******************
        logger.log(LogType::DEBUG, cgiPath);
        // logger.log(LogType::DEBUG, );
        // logger.log(LogType::DEBUG, cgiPath);
        // auto find = req.getHeaders().find("x-secret-header-for-test");
        // if(!find->second.empty())
        // {
        //     int i = 0;
        //     for(auto j:find->second)
        //     {
        //         args[i] = (char *)j.c_str();
        //         i++;
        //     }
        // }
        // char *i = "1";
        // args[0] = i;
        execve(cgiPath.c_str(), args, const_cast<char* const*>(envarg.data()));
        logger.log(LogType::ERROR, string("execve faled ") + strerror(errno));
    }
    else
    {
        // Parent process
        int status = 0;
        //***********
        close(fd1[0]); // Close reading end of first pipe
        close(fd2[1]); // Close writing end of second pipe
        StringBuffer sb;
        Writer wr(fd1[1], false);
        Reader rd(fd2[0], BUFF_SIZE);
        size_t part = 32*1024;
        size_t i = 0;
        wr.writeString(reqBody);
        while(!wr.empty())
        {
            wr.flush();
            sb.add(rd.getAvail());
            i += part;
            //  logger.log(LogType::DEBUG, "ttt ");
        }
        close(fd1[1]);
        sb.add(rd.getToEnd());
        
        // 
        // logger.log(LogType::DEBUG, "response: " + response);
     //   if(response.empty())
        
        waitpid(pid, &status, 0);
        sb.add(rd.getToEnd());
        response = sb.toStr();
        close(fd2[0]);
    }
    Response res(serv);
    setRes(res, response);
    return res;
}

void CgiProcess::setRes(Response &res,const string & resStr)
{
    StringReader sr(resStr);
    string statusLine = sr.getLine();
    
    vector<string> tmp = split(statusLine, " ");
    res.setStatus(trimStr(tmp[1], " "));

    while(true)
    {
        string line = sr.getLine();
        if(line.empty())
            break;
        parseHeaders(res, line); 
    }
    res.setBody(sr.getToEnd());
}


void CgiProcess::parseHeaders(Response &res,const string &s)
{
    string name;
    string val;
    string ts = trimStr(s, LWS);
    size_t pos = 0;
    size_t e = ts.find(':', pos);
    if (e == string::npos)
        logger.log(LogType::ERROR, "incorrect header format " + s);
    name = ts.substr(pos, e - pos);

    pos = ts.find_first_not_of(LWS, e + 1);
    val = ts.substr(pos, s.size() - pos);
    res.addToHeader(name, val);
}

const map<string, string> &CgiProcess::getEnv() const
{
    return env;
}

const string &CgiProcess::getBody() const
{
    return reqBody;
}
