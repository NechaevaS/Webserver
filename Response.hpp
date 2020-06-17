#pragma once
#include "MyServer.hpp"
#include "Reader.hpp"
#include "Connection.hpp"
#include <string>
#include <map>
#include <iostream>
#include <vector>

class MyServer; 
using namespace std;
string timeToHTML(const time_t &time);
class Response
{
    private:
        map<string, vector<string>> headers;
        string body;
        string protocol;
        string status;
        static map<string, string> status_codes;
        MyServer &serv;
    public:
        Response(MyServer &server);
        ~Response();
        Response &operator=(const Response &);
        const map<string, vector<string>> &getHeaders() const;
        void setHeader(const string &name, const string &val);
        void addToHeader(const string &name, const string &val);
        const string &getBody() const;
        const string &getProtocol() const;
        const string &getStatus() const;
        void setBody(const string &body);
        void setStatus(const string &st);
        void send(Writer &w);
        void updateDate();

    struct Exception: public MyServer::Exception
    {
        Exception(const string &msg) : MyServer::Exception(msg){}
    };
};

ostream &operator<<(ostream &out, const Response &resp);