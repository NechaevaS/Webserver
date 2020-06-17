#include "Logger.hpp"
#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"

Logger::Logger(LogType type, ostream &os) : minLogType(type), os(&os)
{
}
void Logger::log(LogType type, const string &message)
{
    if (type < minLogType)
        return;
    *os << typeToStr(type);
    *os << " : " << message << endl;
}
void Logger::log(LogType type, const Config &conf)
{
    if (type < minLogType)
        return;
    *os << typeToStr(type) << ": ";
    *os << conf;
}
void Logger::log(LogType type, const Request &req)
{
    if (type < minLogType)
        return;
    *os << typeToStr(type) << ": ";
    *os << req;
}

void Logger::log(LogType type, const Response &resp)
{
    if (type < minLogType)
        return;
    *os << typeToStr(type) << ": ";
    *os << resp;
}

void Logger::setOut(ostream &os)
{
    this->os = &os;
}

const string &Logger::typeToStr(LogType type)
{
    string strType;
    switch (type)
    {
    case LogType::ERROR:
        return strType = "ERROR";
    case LogType::DEBUG:
        return strType = "DEBUG";
    case LogType::FATAL:
        return strType = "FATAL";
    default:
        return strType = "UNKNOWN";
    }
}