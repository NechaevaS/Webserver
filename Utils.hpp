#pragma once
#include <vector>
#include <string>

using namespace std;
extern const string LWS;
vector<string> split(const string &str, const string &del);

string trimStr(const string &s, const string &trim);
extern "C"
{
    int ft_atoi(const char *);
    int ft_atoi_base(const char *,int);
    char *ft_itoa(int n);
    char *ft_strdup(const char *);
    int ft_toupper(int c);
}

enum FileType
{
    DoesNotExist,
    File,
    Directory
};
string convertStrToUpWithUdsc(const string &str);
FileType getFileType(const string &path);
string getFileExt(const string &path);
string join(const string &delimiter, const vector<string> &v);
vector<string> refineStrVector(const vector<string> &v);