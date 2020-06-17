#include "MyServer.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Connection.hpp"
#include "Processor.hpp"
#include "Config.hpp"
#include "Utils.hpp"
#include "Logger.hpp"
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <netdb.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std; 

Logger logger(LogType::DEBUG, cerr);
bool stopServer = false;

MyServer::MyServer(const string &filename)
{
	setHostAddr();
	conf = new Config(filename);
	errors.open(conf->getErrorLog());
	logger.setOut(errors);
	logger.log(LogType::DEBUG, *conf);
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		throw Exception("Socket not created\n");
	int flags = fcntl(server_fd, F_GETFL);
	if (flags == -1)
		throw Exception("can't get flags for socket\n");
	if (fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw Exception("can't set flags for socket\n");
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(getConf().getPort());
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_fd, (sockaddr *)&addr, sizeof(sockaddr_in)) == -1)
		throw Exception(string("Bind error ") + strerror(errno));
	if (listen(server_fd, SOMAXCONN) == -1)
		throw Exception("Listen error\n");
	connections = new vector<Connection *>;
}

void MyServer::setHostAddr()
{
	char name[256];
	gethostname(name, 256);
	struct hostent *t =	gethostbyname(name);
	if(t->h_addr_list[0] == 0)
		hostAddr = "unknow";
	hostAddr = inet_ntoa(*((struct in_addr*) t->h_addr_list[0]));
}

void MyServer::removeClosedConnections()
{
	vector<Connection *> *new_con = new vector<Connection *>;
	for (auto p : *connections)
	{
		if (p->isClosed())
			delete p;
		else
			new_con->push_back(p);
	}
	delete connections;
	connections = new_con;
}

void MyServer::Run()
{
	int count = 0;
	while (!stopServer)
	{
		int maxFd = server_fd;
		fd_set rfds;
		struct timeval tv;

		FD_ZERO(&rfds);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		removeClosedConnections();
		FD_SET(server_fd, &rfds);
		for (auto p : *connections)
		{
			FD_SET(p->getFd(), &rfds);
			if (maxFd < p->getFd())
				maxFd = p->getFd();
		}
		if (!select(maxFd + 1, &rfds, 0, 0, &tv))
		{
			cout << "no updates " << count++ << endl;
			continue;
		}
		if (FD_ISSET(server_fd, &rfds))
		{
			//new incoming connection
			int new_socket;
			sockaddr_in peer;
			int peerlen = sizeof(peer);

			new_socket = accept(server_fd, (struct sockaddr *)&peer, (socklen_t *)&peerlen);

			if (new_socket < 0)
			{
				if (errno == EAGAIN)
					return;
				throw Exception(string("accept error ") + strerror(errno));
			}
			addConnection(new Connection(*this, new_socket,peer));
			logger.log(LogType::DEBUG, "accepted new connection");
		}
		for (auto p : *connections)
		{
			if(FD_ISSET(p->getFd(), &rfds))
			{
				p->processRequest(true);
				cout << "request processed\n";
			}
		}

	}
}
void 
MyServer::addConnection(Connection *connection)
{
	connections->push_back(connection);
}

// void MyServer::addIoHandler(HandlerIOBase *handler)
// {
// 	ioHandlers.push_back(handler);
// }

const Config &MyServer::getConf() const
{
	return *conf;
}

int MyServer::getServerFd() const
{
	return server_fd;
}

const string &MyServer::getHostAddr() const
{
	return hostAddr;
}

MyServer::~MyServer()
{
	for (auto p : *connections)
	{
		if (p)
			delete p;
	}
	delete connections;
	delete conf;
	close(server_fd);
}
