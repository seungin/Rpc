#include <iostream>

#include "Poco/Net/SocketConnector.h"
#include "Poco/Net/SocketReactor.h"

// Things to remember
// 
// 1. ServiceHandler member variables.
//    * Poco::Net::StreamSocket mSocket;
//    * Poco::Net::SocketReactor& mReactor;
// 
// 2. Registering socket events in ServiceHandler constructor.
//    * using WritableObserver = Poco::Observer<Session, Poco::Net::WritableNotification>;
//    * mReactor.addEventHandler(mSocket, WritableObserver(*this, &Session::onWritable));
// 
// 3. Deregistering socket events in ServiceHandler destructor.
//    * mReactor.removeEventHandler(mSocket, WritableObserver(*this, &Session::onWritable));
// 
// 4. Invoking release function in each event handler.
//    * notification->release();
// 
// 5. Deleting object itself when disconnected.
//    * mSocket.shutdown();
//    * delete this;
// 

class Session
{
	using WritableObserver = Poco::Observer<Session, Poco::Net::WritableNotification>;

public:
	Session(Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& reactor)
		: mSocket(socket), mReactor(reactor), mPeerAddress(mSocket.peerAddress())
	{
		std::cout << "Session has been created.\n - Server: " << mPeerAddress.toString() << "\n\n";
		mReactor.addEventHandler(mSocket, WritableObserver(*this, &Session::onWritable));
	}

	~Session()
	{
		std::cout << "Session has been closed.\n - Server: " << mPeerAddress.toString() << "\n\n";
		mReactor.removeEventHandler(mSocket, WritableObserver(*this, &Session::onWritable));
	}

	void onWritable(Poco::Net::WritableNotification* notification)
	{
		notification->release();

		try
		{
			std::cout << "> ";
			char message[256] = { 0, };
			std::cin.getline(message, sizeof(message) - 1);
			int length = strnlen_s(message, sizeof(message) - 1);

			mSocket.sendBytes(message, length);

			char buffer[256] = { 0, };
			int bytesReceived = mSocket.receiveBytes(buffer, sizeof(buffer) - 1);

			if (bytesReceived > 0)
			{
				std::cout << "From Server: " << buffer << "\n\n";
			}
			else
			{
				mSocket.shutdown();
				delete this;
			}
		}
		catch (Poco::Exception& e)
		{
			std::cout << "Session has been failed.\n";
			mSocket.shutdown();
			delete this;
		}
	}

private:
	Poco::Net::StreamSocket mSocket;
	Poco::Net::SocketReactor& mReactor;

private:
	Poco::Net::SocketAddress mPeerAddress;
};

int main(int argc, char** argv)
{
	std::cout << "Reactor Tcp Client Framework\n\n";

	Poco::Net::SocketAddress address("127.0.0.1", 32452);
	Poco::Net::SocketReactor reactor;
	Poco::Net::SocketConnector<Session> acceptor(address, reactor);

	reactor.run();

	return 0;
}
