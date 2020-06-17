#include "MyServer.hpp"
#include "Response.hpp"
#include "Config.hpp"
#include "Utils.hpp"
#include <ctime>

map<string, string> Response::status_codes = {
    {"100", "Continue"},
    {"101", "Switching Protocols"},
    {"200", "OK"},
    {"201", "Created"},
    {"202", "Accepted"},
    {"203", "Non-Authoritative Information"},
    {"204", "No Content"},
    {"205", "Reset Content"},
    {"206", "Partial Content"},
    {"300", "Multiple Choices"},
    {"301", "Moved Permanently"},
    {"302", "Found"},
    {"303", "See Other"},
    {"304", "Not Modified"},
    {"305", "Use Proxy"},
    {"307", "Temporary Redirect"},
    {"400", "Bad Request"},
    {"401", "Unauthorized"},
    {"402", "Payment Required"},
    {"403", "Forbidden"},
    {"404", "Not Found"},
    {"405", "Method Not Allowed"},
    {"406", "Not Acceptable"},
    {"407", "Proxy Authentication Required"},
    {"408", "Request Timeout"},
    {"409", "Conflict"},
    {"410", "Gone"},
    {"411", "Length Required"},
    {"412", "Precondition Failed"},
    {"413", "Payload Too Large"},
    {"414", "URI Too Long"},
    {"415", "Unsupported Media Type"},
    {"416", "Range Not Satisfiable"},
    {"417", "Expectation Failed"},
    {"426", "Upgrade Required"},
    {"500", "Internal Server Error"},
    {"501", "Not Implemented"},
    {"502", "Bad Gateway"},
    {"503", "Service Unavailable"},
    {"504", "Gateway Timeout"},
    {"505", "HTTP Version Not Supported"}};

Response::Response(MyServer &server):serv(server)
{
    this->protocol = serv.getConf().getProtocol();
    setHeader("Server", serv.getConf().getServName());
    // string methods = join(", ", serv.getConf().getAllowedMethods());
    // setHeader("Allow", methods);

}

const map<string, vector<string>> &Response::getHeaders() const
{
    return headers;
}

const string &Response::getBody() const
{
    return body;
}
const string &Response::getStatus() const
{
    return status;
}
const string &Response::getProtocol() const
{
    return protocol;
}
void Response::setHeader(const string &name, const string &val)
{
    if(headers[name].size() == 0)
        headers[name].push_back(val);
    else
        headers[name][0] = val;
}
void Response::addToHeader(const string &name, const string &val)
{
    headers[name].push_back(val);
}

void Response::setBody(const string &body)
{
    this->body = body;
   
    string s = to_string((int)(body.size()));
    setHeader("Content-Length", s);

}

void Response::setStatus(const string &st)
{
    if(!status_codes.count(st))
        throw Exception("Unknown status code " + st);
    status = st;
}

void Response::send(Writer &w)
{
    updateDate();
    string statusLine = protocol + " " + status + " " + status_codes[status];
    w.writeLine(statusLine);
    for(auto const &h: headers)
    {
        string joinV;
        for(size_t i = 0; i < h.second.size();i++)
        {
            if(i != 0)
                joinV += ",";
            joinV += h.second[i];
        }
        string hLine = h.first + ": " + joinV;
        w.writeLine(hLine);
    }
    w.writeLine("");
    w.writeString(body);
    w.totalFlush();
}

string timeToHTML(const time_t &time)
{
	tm *curr_tm = std::gmtime(&time);
	char date_string[256];

    std::strftime(date_string, 256, "%a, %d %b %Y %H:%M:%S %Z", curr_tm);
    return date_string;
}
void Response::updateDate()
{
    time_t curr_time;
    std::time(&curr_time);
    
    setHeader("Date", timeToHTML(curr_time));
}

Response::~Response(){}

ostream &operator<<(ostream &out, const Response &resp)
{
   
    out << "Response:\n";
    out << "*" << resp.getProtocol() << " " << resp.getStatus() << "*\n";
    for (auto const &i : resp.getHeaders())
    {
        out << "*" << i.first << ":  ";
        for (size_t j = 0; j < i.second.size(); j++)
            out << " " << i.second[j] << ",";
        out << endl;
    }
    out << "*\n";
    out << "*" << resp.getBody() << "*\n";
    return out;
}
