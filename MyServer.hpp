#pragma once
#include <string>
#include <vector>
#include <fstream>

#define BUFF_SIZE (32*1024)

using namespace std;
class Logger;
extern Logger logger;
class Connection;
class Config;

extern bool stopServer;
class MyServer
{
	private:
	vector<Connection *> *connections;
	int server_fd;
	Config *conf;
	string hostAddr;
	void setHostAddr();
	ofstream errors;
	
	public:
	MyServer(const string &filename);
	~MyServer();
	void Run();
	void addConnection(Connection *connection);
	void removeClosedConnections();
	const Config &getConf() const;
	int getServerFd() const;
	const string &getHostAddr() const;

	class Exception
	{
	public:
		Exception(const string &mes)
		{
			message = mes;
		}
		~Exception(){};
		const string &GetMessage() const
		{
			return message;
		}

	private:
		string message;
	};
};
