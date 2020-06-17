#pragma once
#include <string>
#include "MyServer.hpp"

class StringReader
{
    private:
        
        std::string str;
        size_t pos;

    public:
        StringReader(const std::string &str);
        ~StringReader();
        bool empty() const;
        std::string getLine();
        std::string getNBytes(size_t n);
        std::string getToEnd();
        std::string getAvail();
    struct Exception: public MyServer::Exception
    {
        Exception(const string &msg) : MyServer::Exception(msg){}
    };
};