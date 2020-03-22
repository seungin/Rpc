#include <iostream>
#include <thread>
#include <vector>

#include "Poco/DateTimeFormatter.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Timestamp.h"

void DoSend(void)
{
	Poco::Net::SocketAddress address("127.0.0.1", 514);
	Poco::Net::DatagramSocket socket;
	socket.connect(address);

	Poco::Timestamp now;
	std::string msg = Poco::DateTimeFormatter::format(now, "%w %f %H:%M:%S.%i");
	int bytesSent = socket.sendBytes(msg.data(), msg.size());
}

void DoReceive(void)
{
	Poco::Net::SocketAddress address(Poco::Net::IPAddress(), 514);
	Poco::Net::DatagramSocket socket(address);

	{
		const int length = 1024;
		char buffer[length] = { 0, };
		Poco::Net::SocketAddress address;

		int bytesReceived = socket.receiveFrom(buffer, length - 1, address);
		buffer[bytesReceived] = '\0';

		std::cout << "[" << address.toString() << "] " << buffer << std::endl;
	}
}

int main(int argc, char** argv)
{
	std::thread(DoSend).detach();
	DoReceive();
	return 0;
}
