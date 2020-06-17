#pragma once
#include "Request.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include <string>

class Processor
{
    private:
        MyServer &serv;
    public:
        Processor(MyServer &server);
         ~Processor(){}
        Response processRequest(Request &r);
        void setContType(Response &res, const string &path);
        Response processGET(Request &r,const Location &loc);
        Response processPOST(Request &r, const Location &loc);
        Response processPUT(Request &r,const Location &loc);
        Response processDELETE(Request &r,const Location &loc);
        string getLanguageRoot(Request &r);
        string buildPath(Request &r);
};