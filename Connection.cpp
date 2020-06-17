#include "Connection.hpp"
#include "Processor.hpp"
#include "Logger.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Connection::Connection(MyServer &serv, int fd, const sockaddr_in &peer) : serv(serv)
{
    this->fd = fd;
    this->addr = peer.sin_addr;
    this->port = peer.sin_port;
    this->addrStr = inet_ntoa(addr);
    this->closed = false;
}

Connection::~Connection()
{
}

in_addr Connection::getAddr() const
{
    return addr;
}

uint Connection::getPort() const
{
    return port;
}

int Connection::getFd() const
{
    return fd;
}

const string &Connection::getAddrStr() const
{
    return addrStr;
}

bool Connection::isClosed()
{
    return (closed);
}

void Connection::processRequest(bool closeConnection)
{
    try
    {
        Reader reader(fd, BUFF_SIZE);
        if (!reader.empty())
        {
            Request request(reader);
            request.setInAddr(addrStr);
            logger.log(LogType::DEBUG, request);
            Processor processor(serv);
            Response response = processor.processRequest(request);
            if (closeConnection)
                response.setHeader("Connection", "close");
            Writer writer(fd);
            logger.log(LogType::DEBUG, response);
            response.send(writer);
        }
        if (closeConnection)
        {
            close(fd);
            closed = true;
        }
    }
    catch (const  MyServer::Exception &e)
    {
        Response response(serv);
        response.setStatus("400");
        Writer writer(fd, 1024);
        response.send(writer);
        close(fd);
        closed = true;
        logger.log(LogType::DEBUG, "caught exception " + e.GetMessage());
    }
}