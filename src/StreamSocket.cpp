#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/StreamCopier.h"

#include <iostream>

int main(int argc, char** argv)
{
	Poco::Net::SocketAddress addr("www.appinf.com", 80);
	Poco::Net::StreamSocket socket(addr);
	Poco::Net::SocketStream stream(socket);

	stream << "GET / HTTP/1.1\nHost: www.appinf.com\n\n";
	stream.flush();

	Poco::StreamCopier::copyStream(stream, std::cout);
	return 0;
}
