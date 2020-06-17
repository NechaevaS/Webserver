#include "Writer.hpp"
#include "MyServer.hpp"
#include "Logger.hpp"
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cerrno>
#include <fcntl.h>

Writer::Writer(int fd, bool blocking)
{
    this->fd = fd;
    this->pos = 0;
    if (!blocking)
    {
        int flags = fcntl(fd, F_GETFL);
        if (flags == -1)
            throw Exception("can't get flags for file descriptor\n");
        if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
            throw Exception("can't set flags for file descriptor\n");
    }
}

Writer::~Writer() {}

void Writer::flush()
{
    if (empty())
        return;
    struct timeval tv;
    fd_set wfds;

    FD_ZERO(&wfds);
    tv.tv_sec = 0;
    tv.tv_usec = 1;

    FD_SET(fd, &wfds);

    if (!select(fd + 1, 0, &wfds, 0, &tv))
    {
        return;
    }
    int t = write(fd, buff.data() + pos, buff.size() - pos);

    if (t == -1)
    {
        logger.log(LogType::ERROR, string("writer error ") + strerror(errno));
        throw Writer::Exception(strerror(errno));
    }
    pos += t;
    if (empty())
    {
        pos = 0;
        buff.resize(0);
        return;
    }
}

void Writer::totalFlush()
{
    while (!empty())
    {
        flush();
    }
}

bool Writer::empty() const
{
    return pos == buff.size();
}

void Writer::writeString(const std::string &str)
{
    flush();
    if (empty())
        buff = str;
    else
        buff += str;
    flush();
    if (empty())
        buff = "";
}

void Writer::writeLine(const std::string &str)
{
    writeString(str);
    writeString("\r\n");
}
