#pragma once
#include "MyServer.hpp"
#include <string>

using namespace std;

class Writer
{
private:
    string buff;
    int fd;
    size_t pos;

public:
    Writer(int fd, bool blocking = true);
    ~Writer();
    void flush();
    void totalFlush();
    void writeString(const std::string &str);
    void writeLine(const std::string &str);
    bool empty() const;
    struct Exception : public MyServer::Exception
    {
        Exception(const string &msg) : MyServer::Exception(msg) {}
    };
};
// class Writer
// {
// private:
//     char *buff;
//     size_t buffLen;
//     int fd;
//     size_t pos;
//     size_t nWrite = 0;

// public:
//     Writer(int fd, size_t len = 1024, bool blocking = true);
//     ~Writer();
//     void flush();
//     void writeString(const std::string &str);
//     void writeLine(const std::string &str);
//     bool empty() const;
//     struct Exception : public MyServer::Exception
//     {
//         Exception(const string &msg) : MyServer::Exception(msg) {}
//     };
// };