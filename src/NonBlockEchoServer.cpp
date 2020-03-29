#include <iostream>

#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"

int main(int argc, char** argv)
{
	std::cout << "NonBlock Echo Server\n\n";

	Poco::Net::SocketAddress address("127.0.0.1", 32452);
	Poco::Net::ServerSocket serverSocket(address);

	Poco::Net::Socket::SocketList connectedList;
	connectedList.push_back(serverSocket);

	while (true)
	{
		Poco::Net::Socket::SocketList readList(connectedList.begin(), connectedList.end());
		Poco::Net::Socket::SocketList writeList(connectedList.begin(), connectedList.end());
		Poco::Net::Socket::SocketList exceptList(connectedList.begin(), connectedList.end());
		Poco::Timespan timeout(1, 0);

		int count = Poco::Net::Socket::select(readList, writeList, exceptList, timeout);

		if (count == 0)
		{
			continue;
		}

		for (auto& readSocket : readList)
		{
			if (readSocket == serverSocket)
			{
				Poco::Net::StreamSocket streamSocket = serverSocket.acceptConnection();
				connectedList.push_back(streamSocket);
				std::cout << "Connection has been accepted. (Size: " << connectedList.size() << ")\n";
			}
			else
			{
				char buffer[256] = { 0, };
				auto& streamSocket = static_cast<Poco::Net::StreamSocket&>(readSocket);
				int bytesReceived = streamSocket.receiveBytes(buffer, sizeof(buffer) - 1);

				if (bytesReceived > 0)
				{
					std::cout << "From Client: " << buffer << '\n';

					int length = strnlen_s(buffer, sizeof(buffer) - 1);
					streamSocket.sendBytes(buffer, length);
				}
				else
				{
					streamSocket.close();

					auto foundIter = std::find_if(connectedList.begin(), connectedList.end(),
						[&readSocket](auto& socket) { return readSocket == socket; });
					
					if (foundIter != connectedList.end())
					{
						connectedList.erase(foundIter);
						std::cout << "Connection has been closed. (Size: " << connectedList.size() << ")\n";
					}
				}
			}
		}
	}

	return 0;
}
