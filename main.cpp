#include "MyServer.hpp"
#include <iostream>
#include <csignal>

void sigHandler( int signum )
{
   cout << "Interrupt signal (" << signum << ") received.\n";
	stopServer = true;
}

int main()
{
	signal(SIGINT, sigHandler);
	try
	{
		MyServer s("first.config");
		s.Run();
	}
	catch (const MyServer::Exception &ex)
	{
		std::cout << ex.GetMessage() << endl;
	}
	return 0;
}