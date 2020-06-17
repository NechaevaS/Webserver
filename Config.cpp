#include "MyServer.hpp"
#include "Config.hpp"
#include "Reader.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;

Config::Config(const string &filename)
{
    parseConf(filename);
}
Config::~Config()
{
}

void Config::validateSetPort()
{
    int in_port;
    if (conf.find("port") == conf.end())
    {
        port = 80;
        return;
    }
    in_port = stoi(conf["port"]);
    if (in_port > 1 && in_port <= 65535)
        port = in_port;
    else
        port = 80;
}
void Config::validateSetRoot()
{
    if (conf.find("root") == conf.end())
        throw Exception("no field 'root' in config file");

    struct stat info;

    if (stat(conf["root"].c_str(), &info) != 0)
        throw Exception("cannot access to " + conf["root"]);
    else if (info.st_mode & S_IFDIR)
        root = conf["root"];
    else
    {
        logger.log(LogType::FATAL, "root directory " + conf["root"] + "doesn't exist");
    }
}

void Config::validateSetIndex()
{
    if (conf.find("index") == conf.end())
    {
        defaultIndex = "index.html";
        return;
    }
    if (conf["index"].size() != 0)
        servName = conf["index"];
    else
        servName = "index.html";
}

unsigned long long Config::validateSetMaxBody(const string &val)
{
    unsigned long long max_body = INT64_MAX;
    if(val.empty())
       return max_body;
    size_t i;
    max_body = stoll(val, &i);
    if (i != val.size())
         logger.log(LogType::ERROR, "incorrect max_body field " + val);
    
    return max_body;
}

void Config::validateSetServName()
{
    if (conf.find("server_name") == conf.end())
    {
        servName = "Sveta's server";
        return;
    }
    if (conf["server_name"].size() != 0)
        servName = conf["server_name"];
    else
        servName = "Sveta's server";
}

void Config::validateSetProtocol()
{
    if (conf.find("protocol") == conf.end())
    {
        protocol = "HTTP/1.1";
        return;
    }
    if (conf["protocol"].size() != 0)
        protocol = conf["protocol"];
    else
        protocol = "HTTP/1.1";
}

void Config::validateSetServLanguage()
{
    const string langPathPref = "language.path.";
    for (auto const &p : conf)
    {
        size_t i = p.first.find(langPathPref);
        if (i != 0)
            continue;
        string lang = p.first.substr(langPathPref.size(), p.first.size() - langPathPref.size());
        if (lang.empty())
        {
            logger.log(LogType::ERROR, "incorrect language field " + p.first);
            continue;
        }
        string path = join("/", {root, p.second});
        if (getFileType(path) != FileType::Directory)
        {
            logger.log(LogType::ERROR, "path " + path + " is not a dirrectory");
            continue;
        }
        languages[lang] = path;
    }
    string defLang = "language.default";
    if (conf.find(defLang) != conf.end())
    {
        if (languages.find(conf[defLang]) == conf.end())
        {
            logger.log(LogType::ERROR, "incorrect language field language.default");
            if (languages.empty())
            {

                defaultLanguage = "";
                languages[defaultLanguage] = root;
            }
            else
                defaultLanguage = languages.begin()->first;
        }
        else
            defaultLanguage = conf[defLang];
    }
    else
    {
        logger.log(LogType::ERROR, "no default language field set");
        if (!languages.empty())
            defaultLanguage = languages.begin()->first;
        else
        {
            defaultLanguage = "";
            languages[defaultLanguage] = root;
        }
    }
}
void Config::validateSetLocations(const map<string, string> &loc)
{

    string alias;
    string cgi_script;
    vector<string> starts_with;
    vector<string> ends_with;
    vector<string> methods;
    vector<string> index;
    unsigned long long max_body;
    map<string, string>::const_iterator t;
    if ((t = loc.find("alias")) == loc.end())
        alias = "/"; // fixme
    else
        alias = t->second;

    if ((t = loc.find("starts_with")) == loc.end())
        starts_with.push_back(""); // fixme
    else
        starts_with = refineStrVector(split(t->second, ";"));

    if ((t = loc.find("ends_with")) == loc.end())
        ends_with.push_back(""); // fixme
    else
        ends_with = refineStrVector(split(t->second, ";"));

    if ((t = loc.find("methods")) == loc.end())
        methods.push_back("/"); // fixme
    else
        methods = refineStrVector(split(t->second, ";"));

    if ((t = loc.find("index")) == loc.end())
        index.push_back(defaultIndex);
    else
        index = refineStrVector(split(t->second, ";"));

    if ((t = loc.find("cgi_script")) == loc.end())
        cgi_script = "/";
    else
        cgi_script = t->second;
    if ((t = loc.find("max_body")) == loc.end())
        max_body = INT64_MAX;
    else
        max_body = validateSetMaxBody(t->second);
    locations.emplace_back(starts_with, ends_with, methods, alias, index, cgi_script, max_body);
}

void Config::validateSetErrorLog()
{
    if (conf.find("error_log") == conf.end())
    {
        error_log = root + "/logs/error.log";
        return;
    }
    else
        error_log = root + conf["error_log"];
    // struct stat info;

    // if (stat(conf["error_log"].c_str(), &info) != 0)
    //     throw Exception("cannot access to " + conf["error_log"]);
    // else if (info.st_mode & S_IFDIR)
    //     root = conf["error_log"];
    // else
    //     throw Exception("error_log directory " + conf["error_log"] + " doesn't exist");
}

unsigned int Config::getPort() const
{
    return port;
}

const string &Config::getProtocol() const
{
    return protocol;
}
const string &Config::getServName() const
{
    return servName;
}
const string &Config::getErrorLog() const
{
    return error_log;
}
const string &Config::getRoot() const
{
    return root;
}

const string &Config::getDefLanguage() const
{
    return defaultLanguage;
}

const map<string, string> &Config::getLanguages() const
{
    return languages;
}


const string &Config::get(const string &key) const
{
    auto i = conf.find(key);
    if (i == conf.end())
        throw Exception("no such of field " + key + " in config file");
    else
        return i->second;
}

map<string, string> Config::parseLocations(Reader &rd)
{
    map<string, string> loc;
    while (!rd.empty())
    {
        string s = rd.getLine();
        s = trimStr(s, LWS);
        if (s == "end_location")
        {
            return loc;
        }
        if (s.size() == 0)
            continue;
        vector<string> v = split(s, "=");
        if (v.size() != 2)
            throw Exception("incorrect nummber of fields for " + v[0]);
        loc[trimStr(v[0], " \t")] = trimStr(v[1], " \t");
    }
    throw Exception("no end_location");
}

void Config::parseConf(int fd)
{
    Reader rd(fd);
    while (!rd.empty())
    {
        string s = rd.getLine();
        s = trimStr(s, LWS);
        if (s == "end_location")
            throw Exception("end location without location");
        else if (s == "location")
        {
            map<string, string> loc = parseLocations(rd);
            validateSetLocations(loc);
            continue;
        }
        if (s.size() == 0)
            continue;
        vector<string> v = split(s, "=");
        if (v.size() != 2)
            throw Exception("incorrect nummber of fields for " + v[0]);
        conf[trimStr(v[0], " \t")] = trimStr(v[1], " \t");
    }

    validateSetPort();
    validateSetRoot();
    validateSetIndex();
    validateSetServName();
    validateSetServLanguage();
    validateSetProtocol();
    validateSetErrorLog();
}
void Config::parseConf(const string &filename)
{
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd == -1)
        throw Exception("cant open config file " + filename + " " + strerror(errno));
    parseConf(fd);
    close(fd);
}

const vector<string> &Config::getAllowedMethods() const
{
    return methods;
}

const list<Location> &Config::getLocations() const
{
    return locations;
}

ostream &operator<<(ostream &out, const Config &c)
{
    out << "CONFIG:\n";
    out << "*" << "server name: " << c.getServName() << endl;
    out << "*" << "port: " <<c.getPort() << endl;
    out << "*" << "root: " << c.getRoot() << endl;
    out << "*" << "error_log: " << c.getErrorLog() << endl;
    for (auto const &loc : c.getLocations())
    {
        out << loc << endl;
    }
    return out;
}