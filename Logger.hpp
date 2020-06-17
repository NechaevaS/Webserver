#pragma once
#include <string>
#include <iostream>
#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"

using namespace std;
enum LogType
{
    DEBUG,
    ERROR,
    FATAL
};

class Logger
{
    private:
    LogType minLogType;
    ostream *os;
    public:
    Logger(LogType type, ostream &os);
    void log(LogType type, const string & message);
    void log(LogType type, const Config &conf);
    void log(LogType type, const Request &req);
    void log(LogType type, const Response &resp);
    ostream &getOut(LogType type);
    void setOut(ostream &os);
    const string &typeToStr(LogType type);
};