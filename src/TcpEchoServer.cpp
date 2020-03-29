#include <iostream>

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"

int main(int argc, char** argv)
{
	Poco::Net::SocketAddress address(32452);
	Poco::Net::ServerSocket serverSocket(address);
	std::cout << "Tcp Echo Server\n\n";

	try
	{
		Poco::Net::StreamSocket socket = serverSocket.acceptConnection();
		std::cout << "Connection has been accepted.\n";

		while (true)
		{
			char buffer[256] = { 0, };
			int bytesReceived = socket.receiveBytes(buffer, sizeof(buffer));
			std::cout << "From Client: " << buffer << '\n';

			if (bytesReceived <= 0)
			{
				std::cout << "Connection has been closed.\n";
				break;
			}

			int length = strnlen_s(buffer, sizeof(buffer) - 1);
			socket.sendBytes(buffer, length);
		}

		socket.close();
	}
	catch (Poco::Exception& e)
	{
		std::cout << "Connection has been failed.\n - " << e.displayText() << '\n';
	}

	return 0;
}
