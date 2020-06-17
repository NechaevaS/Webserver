#pragma once
#include <list>
#include <string>

using namespace std;

class StringBuffer
{
    private:
    list<string> strings;
    size_t totalBytes = 0;
    public:
    StringBuffer() = default;
    void add(const string &);
    string toStr();
};