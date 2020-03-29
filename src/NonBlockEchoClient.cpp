#include <iostream>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"

int main(int argc, char** argv)
{
	std::cout << "NonBlock Echo Client\n\n";

	Poco::Net::SocketAddress address("127.0.0.1", 32452);
	Poco::Net::StreamSocket streamSocket;

	try
	{
		streamSocket.connectNB(address);

		Poco::Timespan timeout(1, 0);

		while (!streamSocket.poll(timeout, Poco::Net::Socket::SELECT_WRITE))
		{
			std::cout << "Waiting for Connection...\n";
		}
		std::cout << "Connection has been established.\n";

		std::cout << "> ";
		char message[256] = { 0, };
		std::cin.getline(message, sizeof(message) - 1);
		int length = strnlen_s(message, sizeof(message) - 1);

		streamSocket.sendBytes(message, length);

		while (!streamSocket.poll(timeout, Poco::Net::Socket::SELECT_READ))
		{
			std::cout << "Waiting for Reception...\n";
		}

		char buffer[256] = { 0, };
		int bytesReceived = streamSocket.receiveBytes(buffer, sizeof(buffer) - 1);

		if (bytesReceived <= 0)
		{
			std::cout << "Connection has been closed.\n";
		}

		std::cout << "From Server: " << buffer << '\n';

		streamSocket.close();
	}
	catch (Poco::Exception& e)
	{
		std::cout << "Connection has been failed.\n - " << e.displayText() << '\n';
	}

	return 0;
}
