#include "Request.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include "StringBuffer.hpp"
#include <vector>
#include <locale>
#include <algorithm>

map<string, void (Request::*)(const string &name, const string &val)> Request::parsers =
    {
        {"accept-charsets", &Request::parseValList},
        {"accept-encoding", &Request::parseValList},
        {"accept-language", &Request::parseValList},
        {"authorization", &Request::parseValList},
        {"content-lenguage", &Request::parseValSingle},
        {"content-length", &Request::parseValSingle},
        {"content-location", &Request::parseValSingle},
        {"content-type", &Request::parseValSingle},
        {"host", &Request::parseValSingle},
        {"referer", &Request::parseValSingle},
        {"transfer-encoding", &Request::parseValList},
         {"x-secret-header-for-test", &Request::parseValList},
        {"user-agent", &Request::parseValSingle}};
// namespace
// {

Request::Request(Reader &reader)
{
    string rqLine = reader.getLine();
    vector<string> tmp = split(rqLine, " ");
    if (tmp.size() != 3)
        throw Exception("Incorrect request line");
    method = tmp[0];
    uri = RequestURI(tmp[1]);
    httpVersion = trimStr(tmp[2], LWS);
    while (true)
    {
        string s = reader.getLine();
        if (s.empty())
            break;
        parseHeader(s);
    }
    if (headers.count("transfer-encoding"))
    {
        StringBuffer sb;
        int chunkSize = 0;
        do
        {
            string tmp = trimStr(reader.getLine(), LWS);
            chunkSize = ft_atoi_base(tmp.c_str(), 16);
            sb.add(reader.getNBytes(chunkSize));
            reader.getLine();
        } while (chunkSize);
        body = sb.toStr();
    }
    else if (headers.count("content-length"))
    {
        size_t bodySize = stoi(headers[string("content-length")][0]);
        if (bodySize != 0)
        {
            body = reader.getNBytes(bodySize);
        }
    }
}

const string &Request::getMethod() const
{
    return method;
}
const RequestURI &Request::getUri() const
{
    return uri;
}
const string &Request::getHttpVersion() const
{
    return httpVersion;
}
const map<string, vector<string>> &Request::getHeaders() const
{
    return headers;
}
const string &Request::getBody() const
{
    return body;
}

const string &Request::getInAddr() const
{
    return inAddr;
}

const string &Request::getRoot() const
{
    return root;
}

void Request::setUri(const RequestURI &u)
{
    this->uri = u;
}

void Request::setInAddr(const string &in_addr)
{
    this->inAddr = in_addr;
}

void Request::setRoot(const string &root)
{
    this->root = root;
}

ostream &operator<<(ostream &out, const Request &r)
{
    out << "Request:\n";
    out << "*" << r.getMethod() << " " << r.getUri() << " " << r.getHttpVersion() << "*\n";
    for (auto const &i : r.getHeaders())
    {
        out << "*" << i.first << ":  ";
        for (size_t j = 0; j < i.second.size(); j++)
            out << " " << i.second[j] << ",";
        out << endl;
    }
    out << "*\n";
    out << "*" << r.getBody() << "*\n";
    return out;
}

void Request::parseValList(const string &name, const string &val)
{
    vector<string> res = split(val, ",");
    for (size_t i = 0; i < res.size(); i++)
        res[i] = trimStr(res[i], LWS);
    headers[name].insert(headers[name].end(), res.begin(), res.end());
}

void Request::parseValSingle(const string &name, const string &val)
{
    if (headers[name].size() == 0)
        headers[name].push_back(trimStr(val, LWS));
    else
        headers[name][0] = trimStr(val, LWS);
}

void Request::parseHeader(const string &s)
{
    string name;
    string val;
    string ts = trimStr(s, LWS);
    size_t pos = 0;
    size_t e = ts.find(':', pos);
    if (e == string::npos)
        throw Exception("Incorrect header format: " + s);
    name = ts.substr(pos, e - pos);
    transform(name.begin(), name.end(), name.begin(), ::tolower);
    pos = ts.find_first_not_of(LWS, e + 1);
    val = ts.substr(pos, s.size() - pos);
    if (parsers.count(name) == 0)
    {
        string message = "unsupported header type: " + name;
        logger.log(LogType::DEBUG, message);
        return;
    }
    (this->*parsers[name])(name, val);
    // vector<string> valV = parseValList(val);
    // if(headers.count(name) == 0)
    //     headers[name] = valV;
    // else
    //     headers[name].insert(headers[name].end(), valV.begin(), valV.end());

    // cout << name << "*\n";
    // cout << val << "*\n";
}
//} // namespace

// string Request::UriToString()
// {
//     string UriStr = path;
//     if (!r.getUri().getQuery().empty())
//         newUri += "?" + r.getUri().getQuery();
//     if (!r.getUri().getFragment().empty())
//         newUri += "#" + r.getUri().getFragment();
//     return UriStr;
// }