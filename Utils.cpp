#include "Utils.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>

using namespace std;

const string LWS = " \t\r\n";
vector<string> split(const string &str, const string &del)
{
    vector<string> res;
    for (size_t pos = 0; pos < str.size();)
    {
        size_t s = str.find_first_not_of(del, pos);
        if (s == string::npos)
            break;
        size_t e = str.find_first_of(del, s);
        if (e == string::npos)
            e = str.size();
        res.push_back(str.substr(s, e - s));
        pos = e;
    }
    return res;
}

string trimStr(const string &s, const string &trim)
{
    size_t start = s.find_first_not_of(trim, 0);
    if (start == string::npos)
        return "";
    size_t end = s.size() - 1;
    for (; end >= start; end--)
    {
        size_t i = 0;
        for (; i < trim.size() && s[end] != trim[i]; i++)
            ;
        if (i == trim.size())
            break;
    }
    string catS = s.substr(start, end + 1 - start);
    return catS;
}

FileType getFileType(const string &path)
{
    struct stat info;

    if (stat(path.c_str(), &info) != 0)
        return DoesNotExist;
    else if (info.st_mode & S_IFDIR)
        return Directory;
    else
        return FileType::File;
}

string getFileExt(const string &path)
{
    vector<string> pathParts = split(path, "/");
    string ext;
    if (pathParts.size() == 0)
        return "";
    string fileName = pathParts[pathParts.size() - 1];
    vector<string> fileParts = split(fileName, ".");
    if (fileParts.size() == 0)
        return "";
    return fileParts[fileParts.size() - 1];
}

string join(const string &delimiter, const vector<string> &v)
{
    string res;
    for (size_t i = 0; i < v.size(); i++)
    {
        if (i != 0)
            res += delimiter;
        res += v[i];
    }
    return res;
}

vector<string> refineStrVector(const vector<string> &v)
{
    vector<string> res;
    for (auto const &e : v)
    {
        string tmp = trimStr(e, LWS);
        if (tmp.empty())
            continue;
        res.push_back(tmp);
    }
    return res;
}

string convertStrToUpWithUdsc(const string &str)
{
    string res;
    res.reserve(str.size());
    for(auto i:str)
    {
        if(i == '-')
            res.push_back('_');
        else
            res.push_back(ft_toupper(i));
    }
    return res;
}