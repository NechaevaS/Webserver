#pragma once
#include "Request.hpp"
#include "Response.hpp"
#include <map>
#include <string>
#include <vector>

using namespace std;

class CgiProcess
{
    private:
    map<string, string> env;
    string cgiPath;
    string cgiName;
    string path;
    string reqBody;
    MyServer &serv;
    Request &req;
    void setRes(Response &res,const string & resStr);
    void parseHeaders(Response &res,const string &s);
    void HTTPheadersToEnv();
    public:
    CgiProcess(MyServer & serv, Request &req, const string &cgiPath, const string &cgiName);
    ~CgiProcess();
    Response processCGI();
    const map<string, string> &getEnv() const;
    const string &getBody() const;
};