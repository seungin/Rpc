#include <iostream>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/DatagramSocket.h"

int main(int argc, char** argv)
{
	Poco::Net::SocketAddress address("127.0.0.1", 32452);
	Poco::Net::DatagramSocket socket(address);
	std::cout << "Udp Echo Server\n\n";

	try
	{
		while (true)
		{
			char buffer[256] = { 0, };
			Poco::Net::SocketAddress address;
			int bytesReceived = socket.receiveFrom(buffer, sizeof(buffer) - 1, address);

			if (bytesReceived > 0)
			{
				std::cout << "From " << address.toString() << " " << buffer << '\n';

				int length = strnlen_s(buffer, sizeof(buffer) - 1);
				socket.sendTo(buffer, length, address);
			}
		}
	}
	catch (Poco::Exception& e)
	{
		std::cout << "Connection has been failed.\n - " << e.displayText() << '\n';
	}

	return 0;
}
