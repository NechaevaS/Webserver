#include "RequestURI.hpp"

RequestURI::RequestURI(const string &uri)
{
    uriStr = uri;
    size_t start = 0;
    size_t end = uriStr.find_first_of("?#", 0);
    if (end == string::npos)
    {
        path = uriStr;
        return;
    }
    path = uriStr.substr(start, end - start);
    start = end;
    if (uriStr[start] == '?')
    {
        end = uriStr.find_first_of("#", 0);
        if (end == string::npos)
        {
            query = uriStr.substr(start + 1, end - start - 1);
            return;
        }
    }
    fragment = uriStr.substr(end + 1, uriStr.size() - end - 1);
}

RequestURI::RequestURI(const string &path, const string &query, const string &fragment)
{
    this->path = path;
    this->query = query;
    this->fragment = fragment;
    updateUriStr();

}
RequestURI::~RequestURI() {}

const string &RequestURI::getPath() const
{
    return path;
}

void RequestURI::updateUriStr()
{
    uriStr = path;
    if (query.size() != 0)
        uriStr += "?" + query;
    if (fragment.size() != 0)
        uriStr += "#" + fragment;
}
const string &RequestURI::getQuery() const
{
    return query;
}

const string &RequestURI::getFragment() const
{
    return fragment;
}

const string &RequestURI::getUriStr() const
{
    return uriStr;
}

ostream &operator<<(ostream &out, const RequestURI &uri)
{
    out << uri.getPath();
    if (uri.getQuery().size() != 0)
        out << "?" << uri.getQuery();
    if (uri.getFragment().size() != 0)
        out << "#" << uri.getFragment();
    return out;
}