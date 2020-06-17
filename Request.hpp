#pragma once
#include "MyServer.hpp"
#include "Reader.hpp"
#include "RequestURI.hpp"
#include <string>
#include <map>
#include <iostream>
#include <vector>


using namespace std;
class Request
{
private:
    string inAddr;
    string method;
    RequestURI uri;
    string root;
    string httpVersion;
    map<string, vector<string>> headers;
    string body;
    void parseHeader(const string &s);
    static map<string, void (Request::*)(const string &name, const string &val)> parsers;

public:
    Request(Reader &reader);
    ~Request(){};
    const string &getMethod() const;
    const RequestURI &getUri() const;
    const string &getHttpVersion() const;
    const map<string, vector<string>> &getHeaders() const;
    const string &getBody() const;
    const string &getRoot() const;
    const string &getInAddr() const;
    void parseValList(const string &name, const string &val);
    void parseValSingle(const string &name, const string &val);
    void setUri(const RequestURI &n);
    void setInAddr(const string &in_addr);
    void setRoot(const string &root);
 
    struct Exception: public MyServer::Exception
    {
        Exception(const string &msg) : MyServer::Exception(msg){}
    };
};

ostream &operator<<(ostream &out, const Request &r);

