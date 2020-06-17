#include "StringBuffer.hpp"

using namespace std;

void StringBuffer::add(const string &str)
{
    if(str.empty())
        return;
    strings.emplace_back(str);
    totalBytes += str.size();
}

string StringBuffer::toStr()
{
    size_t newStrSize = 0;
    for(auto i:strings)
    {
        newStrSize +=i.size();
    }
    string res;
    res.reserve(newStrSize);
    for(auto i:strings)
    {
        res += i;
    }
    return res;
}
