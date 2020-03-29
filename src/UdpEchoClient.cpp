#include <iostream>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DatagramSocket.h"

int main(int argc, char** argv)
{
	Poco::Net::SocketAddress address("127.0.0.1", 32451);
	Poco::Net::DatagramSocket socket(address);
	std::cout << "Udp Echo Client\n\n";

	try
	{
		while (true)
		{
			std::cout << "> ";
			char message[256] = { 0, };
			std::cin.getline(message, sizeof(message) - 1);
			int length = strnlen_s(message, sizeof(message) - 1);

			socket.sendTo(message, length, Poco::Net::SocketAddress("127.0.0.1", 32452));

			char buffer[256] = { 0, };
			Poco::Net::SocketAddress address;
			int bytesReceived = socket.receiveFrom(buffer, sizeof(buffer) - 1, address);

			if (bytesReceived <= 0)
			{
				std::cout << "Connection has been disconnected.\n";
				break;
			}

			std::cout << "From " << address.toString() << " " << buffer << '\n';
		}
	}
	catch (Poco::Exception& e)
	{
		std::cout << "Connetion has been failed.\n - " << e.displayText() << '\n';
	}

	return 0;
}
