#include "StringReader.hpp"
#include "MyServer.hpp"

using namespace std;

StringReader::StringReader(const string &str) : str(str)
{
    pos = 0;
}
StringReader::~StringReader() {}

bool StringReader::empty() const
{
    return pos == str.size();
}

std::string StringReader::getLine()
{
    size_t e = str.find_first_of("\r\n", pos);
    if (e == string::npos)
        e = str.size();
    string res = str.substr(pos, e - pos);
    pos = e;
    if( pos < str.size() && str[pos] == '\r')
        pos++;
    if( pos < str.size() && str[pos] == '\n')
        pos++;
    return res;
}
std::string StringReader::getNBytes(size_t n)
{
    if(n > str.size() - pos)
        throw Exception("out of bounds exeption");
    size_t t = pos;
    pos +=n;
    return str.substr(t, n);
}
std::string StringReader::getToEnd()
{
    return getNBytes(str.size()- pos);
}
std::string StringReader::getAvail()
{
    return getToEnd();
}
