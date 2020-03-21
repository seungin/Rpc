#include <iostream>

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"

int main(int argc, char** argv)
{
	Poco::Net::ServerSocket serverSocket(19703);

	while (true)
	{
		Poco::Net::StreamSocket socket = serverSocket.acceptConnection();
		Poco::Net::SocketStream stream(socket);

		stream << "HTTP/1.0 200 OK\n"
			"Content-Type: text/html\n"
			"<html><head><title>Web Server</title></head>"
			"<body><h1>Hello, World!</h1></body></html>"
			<< std::flush;
	}

	return 0;
}
