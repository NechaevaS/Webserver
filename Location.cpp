#include "Location.hpp"
#include <iostream>
#include "Utils.hpp"

Location::Location(const vector<string> &starts_with,
         const vector<string> &ends_with,
         const vector<string> &methods,
         const string &alias,
         const vector<string> &index,
         const string & cgi_script,
         unsigned long long max_body)
{
    this->starts_with = starts_with;
    this->ends_with = ends_with;
    this->methods = methods;
    this->alias = alias;
    this->index = index;
    this->cgi_script = cgi_script;
    this->max_body = max_body;
}

const string &Location::getAlias() const
{
    return alias;
}

const vector<string> &Location::getStartWith() const
{
    return starts_with;
}

const vector<string> &Location::getEndtWith() const
{
    return ends_with;
}

const vector<string> &Location::getMethods() const
{
    return methods;
}

const vector<string> &Location::getIndex() const
{
    return index;
}


unsigned long long Location::getMaxBody() const
{
    return max_body;
}

const string &Location::getCgiScript() const
{
    return cgi_script;
}

bool Location::isMatchMethod(const string &method) const
{
    bool matched = false;
    for(auto const &m:methods)
    {
        if(m == method)
        {
            matched = true;
            break;
        }
    }
    return matched;
}
string normalisePath(const string &path)
{
    string normPath;
    normPath.reserve(path.size());
    normPath.push_back(path[0]);
    for(size_t j = 1; j < path.size(); j++)
    {
        if(path[j - 1] == '/' && path[j] == '/')
            continue;
        normPath.push_back(path[j]);
    }
    return normPath;
}

vector<string> Location::matchesUri(const string &uri) const
{
    vector<string> res;
    for(auto const &s:starts_with)
    {  
        string t = uri.substr(0, s.length());
        if(s == t)
        {
            for(auto const &e:ends_with)
            {   
                if(uri.length() < e.length())
                    continue;
                if(e == uri.substr(uri.length() - e.length() , e.length()))
                {
                    string new_uri = alias + uri.substr(s.length());
                    new_uri = normalisePath(new_uri);
                    res.push_back(new_uri);
                }
            }
        }
    }
    return res;
}

ostream &operator<<(ostream &out, const Location &l)
{
    out << "location" << endl;
    out << "\tstarts_with = " << join(";", l.getStartWith()) << endl;
    out << "\tends_with = " << join(";", l.getEndtWith()) << endl;
    out << "\tmethods_with = " << join(";", l.getMethods()) << endl;
    out << "\tindex = " << join(";", l.getIndex()) << endl;
    out << "\talias = " << l.getAlias() << endl;
    out << "\tcgi_script = " << l.getCgiScript() << endl;
    return out;
}