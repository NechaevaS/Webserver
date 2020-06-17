#pragma once
#include <string>
#include <iostream>

using namespace std;
class RequestURI
{
    private:
        string uriStr;
        string path;
        string query;
        string fragment;
        void updateUriStr();
    public:
        RequestURI() = default;
        RequestURI(const string &uri);
        RequestURI(const string &path, const string &query, const string &fragment);
        ~RequestURI();
        const string &getPath() const; 
        const string &getQuery() const;
        const string &getFragment() const;
        const string &getUriStr() const;
};      
ostream &operator<<(ostream &out, const RequestURI &uri);
