#include "Reader.hpp"
#include "StringBuffer.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <sys/select.h>

Reader::Reader(int fd, size_t len)
{
    this->fd = fd;
    this->buffLen = len;
    this->pos = 0;

    buffUpdate();
}

Reader::~Reader()
{
}

bool Reader::empty() const
{
    return eof() && pos == buff.size();
}

bool Reader::eof() const
{
    return endOfFile;
}

size_t Reader::buffUpdate()
{
    if (endOfFile)
        return 0;
    if (pos == buff.size())
    {
        struct timeval tv;
        fd_set rfds;

        FD_ZERO(&rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 1;

        FD_SET(fd, &rfds);

        if (!select(fd + 1, &rfds, 0, 0, &tv))
        {
            return 0;
        }
        char *tmp = new char[buffLen];
        int nRead;
        if ((nRead = read(fd, tmp, buffLen)) == -1)
        {
            logger.log(LogType::FATAL, string("can't read ") + strerror(errno));
            throw Exception(string("can't read ") + strerror(errno));
        }
        if (nRead == 0)
        {
            endOfFile = true;
        }
        buff = string(tmp, nRead);
        pos = 0;

        delete[] tmp;
    }
    return buff.size() - pos;
}

std::string Reader::getLine()
{
    StringBuffer sb;
    while (!endOfFile)
    {
        buffUpdate();
        size_t e = buff.find_first_of("\n", pos);

        if (e == string::npos)
        {
            sb.add(buff.substr(pos, buff.size() - pos));
            pos = buff.size();
            continue;
        }
        sb.add(buff.substr(pos, e - pos));
        pos = e + 1;
        break;
    }
    string tmp = sb.toStr();
    if(tmp[tmp.size() - 1] == '\r')
        return tmp.substr(0, tmp.size() - 1);
    return tmp;
}

std::string Reader::getNBytes(size_t n)
{
    std::string res;
    res.reserve(n);
    while (!eof() && res.size() < n)
    {
        buffUpdate();
        size_t needToRead = n - res.size();
        size_t availInBuff = buff.size() - pos;
        size_t toRead = (needToRead < availInBuff) ? needToRead : availInBuff;

        res += buff.substr(pos, toRead);
        pos += toRead;
    }
    return res;
}

std::string Reader::getToEnd()
{
    StringBuffer sb;
    while (!eof())
    {
        size_t nBytes = buffUpdate();
        sb.add(getNBytes(nBytes));
    }
    return sb.toStr();
}

std::string Reader::getAvail()
{
    StringBuffer sb;
    while (!eof())
    {
        size_t nBytes = buffUpdate();
        if(nBytes == 0)
            break;
        sb.add(buff.substr(pos, buff.size() - pos));
        pos = buff.size();
    }
    return sb.toStr();

}
