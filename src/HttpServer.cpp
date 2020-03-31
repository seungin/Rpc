#include <iostream>

#include "Poco/File.h"

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class ErrorRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
	virtual void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
		response.setContentType("text/html");

		std::ostream& stream = response.send();
		stream << "<html>"
			<< "<head><title>Web Server powered by Poco</title></head>"
			<< "<body><h1>Error, Not Found.</h1><br /></body>"
			<< "</html>";
	}
};

class FileRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
	virtual void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
	{
		try
		{
			std::string path = "." + request.getURI();
			response.sendFile(path, "text/html");
		}
		catch (Poco::Exception& e)
		{
			std::cout << e.displayText() << "\n\n";
		}
	}
};

class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
	virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override
	{
		std::cout << "Request: URI( " << request.getURI() << " )\n";

		std::string path = "." + request.getURI();
		Poco::File file(path);

		if (!file.exists() || !file.isFile())
		{
			return new ErrorRequestHandler;
		}

		return new FileRequestHandler;
	}
};

int main(int argc, char** argv)
{
	std::cout << "Http Server Framework\n\n";

	int maxThreads = 1;

	Poco::ThreadPool::defaultPool().addCapacity(maxThreads);

	auto serverParams = new Poco::Net::HTTPServerParams;
	serverParams->setMaxThreads(maxThreads);
	serverParams->setMaxQueued(100);

	Poco::Net::SocketAddress address("127.0.0.1", 32452);
	Poco::Net::ServerSocket serverSocket(address);
	Poco::Net::HTTPServer server(new RequestHandlerFactory, serverSocket, serverParams);

	server.start();
	getchar();

	server.stop();
	return 0;
}
