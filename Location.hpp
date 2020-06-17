#pragma once
#include <string>
#include <vector>

using namespace std;

class Location
{
    private:
        string alias;
        vector<string> index;
        vector<string> starts_with;
        vector<string> ends_with;
        vector<string> methods;
        string cgi_script;
        unsigned long long max_body;
    public:
        Location(const vector<string> &starts_with,
            const vector<string> &ends_with,
            const vector<string> &methods, 
            const string &alias,
            const vector<string> &index, const string & cgi_script,
            unsigned long long max_body);
        const string &getAlias() const;
        const vector<string> &getStartWith() const;
        const vector<string> &getEndtWith() const;
        const vector<string> &getMethods() const;
        const vector<string> &getIndex() const;
        const string &getCgiScript() const;
        unsigned long long getMaxBody() const;
        bool isMatchMethod(const string &method) const;
        vector<string> matchesUri(const string &uri) const;

};

ostream &operator<<(ostream &out, const Location &l);