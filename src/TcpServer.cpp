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
		std::cout << "Session has been closed.\n";
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

void PrintServerStatus(const Poco::Net::TCPServer& server)
{
	std::cout << "Server Status\n";
	std::cout << " - Threads: current( " << server.currentThreads() << " )\n";
	std::cout << " - Connections: total( " << server.totalConnections() << " ), "
		<< "queued( " << server.queuedConnections() << " ), "
		<< "current( " << server.currentConnections() << " ), "
		<< "max-concurrent( " << server.maxConcurrentConnections() << " )\n\n";
}

int main(int argc, char** argv)
{
	std::cout << "Tcp Server Framework\n\n";

	Poco::Net::SocketAddress address("127.0.0.1", 32452);
	Poco::Net::ServerSocket serverSocket(address);

	auto serverParams = new Poco::Net::TCPServerParams;
	serverParams->setMaxQueued(4);
	serverParams->setMaxThreads(4);

	Poco::Net::TCPServer server(new SessionFactory, serverSocket, serverParams);
	std::cout << "Max Server Threads: " << server.maxThreads() << "\n\n";

	server.start();

	while (true)
	{
		Poco::Thread::sleep(2000);
		PrintServerStatus(server);
	}

	return 0;
}
