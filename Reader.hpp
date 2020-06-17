#pragma once
#include <string>
#include "MyServer.hpp"

class Reader
{
    private:
        string buff;
        size_t buffLen;
        size_t pos;
        int fd;
        bool endOfFile = false;
        size_t buffUpdate();

    public:
        Reader(int fd, size_t len = 1024);
        ~Reader();
        bool empty() const;
        bool eof() const;
        std::string getLine();
        std::string getNBytes(size_t n);
        std::string getToEnd();
        std::string getAvail();
    struct Exception: public MyServer::Exception
    {
        Exception(const string &msg) : MyServer::Exception(msg){}
    };
};