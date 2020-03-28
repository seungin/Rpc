#include <iostream>

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"

int main(int argc, char** argv)
{
	Poco::Net::StreamSocket socket;

	try
	{
		Poco::Net::SocketAddress address("127.0.0.1", 32452);
		socket.connect(address);
		std::cout << "Tcp Echo Client\n - Connection has been connected.\n\n";

		while (true)
		{
			std::cout << "> ";
			char message[256] = { 0, };
			std::cin.getline(message, sizeof(message) - 1);
			int length = strnlen_s(message, sizeof(message) - 1);

			socket.sendBytes(message, length);

			char buffer[256] = { 0, };
			int bytesReceived = socket.receiveBytes(buffer, sizeof(buffer));

			if (bytesReceived <= 0)
			{
				std::cout << "Connection has been disconnected.\n";
				break;
			}

			std::cout << "From Server: " << buffer << '\n';
		}

		socket.close();
	}
	catch (Poco::Exception& e)
	{
		std::cout << "Connection has been failed.\n - " << e.displayText() << '\n';
	}

	return 0;
}
