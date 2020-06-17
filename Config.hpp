#pragma once
#include <map>
#include <string>
#include <vector>
#include <list>
#include "MyServer.hpp"
#include "Location.hpp"
#include "Reader.hpp"
using namespace std;

class Config
{
private:
    map<string, string> conf;

    unsigned int port;
    string protocol;
    string root;
    string servName;
    list<Location> locations;
    string defaultLanguage;
    string defaultIndex;
    map<string, string> languages;
    vector<string> methods = {"GET", "PUT"};
    string error_log;

    void parseConf(int fd);
    void parseConf(const string &filename);
    map<string, string> parseLocations(Reader &rd);
    const string &get(const string &key) const;
    void validateSetPort();
    void validateSetProtocol();
    void validateSetRoot();
    void validateSetIndex();
    void validateSetServName();
    void validateSetServLanguage();
    void validateSetLocations(const map<string, string> &loc);
    void validateSetErrorLog();
    unsigned long long validateSetMaxBody(const string &val);

public:
    Config(const string &filename);
    ~Config();
    unsigned int getPort() const;
    const string &getServName() const;
    const string &getProtocol() const;
    const string &getErrorLog() const;
    const string &getRoot() const;
    const string &getDefLanguage() const;
    const map<string, string> &getLanguages() const;
    const vector<string> &getAllowedMethods() const;
    const list<Location> &getLocations() const;
    struct Exception : public MyServer::Exception
    {
        Exception(const string &msg) : MyServer::Exception(msg) {}
    };
};
ostream &operator<<(ostream &out, const Config &c);