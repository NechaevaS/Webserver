#pragma once
#include <string>
#include "Request.hpp"
#include "Reader.hpp"
#include "Writer.hpp"
#include "MyServer.hpp"
#include <netinet/in.h>
class MyServer;
class Connection
{
    private:
    	static const size_t bufLen = 1024;
        in_addr addr;
        uint port;
        int fd;
        string addrStr;
        bool closed;
        MyServer &serv;

    public:
        Connection(MyServer &serv, int fd, const sockaddr_in &peer);
        ~Connection();
        in_addr getAddr() const;
        uint getPort() const;
        int getFd() const;
        const string &getAddrStr() const;
        bool isClosed();
        void processRequest(bool closeConnection = false);
};