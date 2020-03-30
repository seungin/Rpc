#include <iostream>

#include "Poco/Net/TcpServer.h"

class Session : public Poco::Net::TCPServerConnection
{
public:
	Session(const Poco::Net::StreamSocket& socket)
		: TCPServerConnection(socket)
	{
		std::cout << "Session has been created.\n";
	}

	virtual ~Session()
	{
		std::cout << "Session has been destroyed.\n";
	}

	virtual void run() override
	{
		try
		{
			char buffer[256] = { 0, };
			int bytesReceived = socket().receiveBytes(buffer, sizeof(buffer) - 1);

			while (bytesReceived > 0)
			{
				std::cout << "From Client: " << buffer << '\n';

				int length = strnlen_s(buffer, sizeof(buffer) - 1);
				socket().sendBytes(buffer, length);

				char buffer[256] = { 0, };
				bytesReceived = socket().receiveBytes(buffer, sizeof(buffer) - 1);
			}
		}
		catch (Poco::Exception& e)
		{
			std::cout << "Connection has benn failed.\n - " << e.displayText() << '\n';
		}
	}
};

class SessionFactory : public Poco::Net::TCPServerConnectionFactory
{
public:
	virtual Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket) override
	{
		return new Session(socket);
	}
};

int main(int argc, char** argv)
{
	std::cout << "Tcp Server Framework\n\n";

	Poco::Net::SocketAddress address("127.0.0.1", 32452);
	Poco::Net::ServerSocket serverSocket(address);

	Poco::Net::TCPServer server(new SessionFactory, serverSocket);
	server.start();

	while (true)
	{
		Poco::Thread::sleep(1);
	}

	return 0;
}
