#include <iostream>

#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/SocketReactor.h"

// Things to remember
// 
// 1. ServiceHandler member variables.
//    * Poco::Net::StreamSocket mSocket;
//    * Poco::Net::SocketReactor& mReactor;
// 
// 2. Registering socket events in ServiceHandler constructor.
//    * using ReadableObserver = Poco::Observer<Session, Poco::Net::ReadableNotification>;
//    * mReactor.addEventHandler(mSocket, ReadableObserver(*this, &Session::onReadable));
// 
// 3. Deregistering socket events in ServiceHandler destructor.
//    * mReactor.removeEventHandler(mSocket, ReadableObserver(*this, &Session::onReadable));
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
	using ReadableObserver = Poco::Observer<Session, Poco::Net::ReadableNotification>;

public:
	Session(Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& reactor)
		: mSocket(socket), mReactor(reactor), mPeerAddress(mSocket.peerAddress())
	{
		std::cout << "Session has been created.\n - Client: " << mPeerAddress.toString() << "\n\n";
		mReactor.addEventHandler(mSocket, ReadableObserver(*this, &Session::onReadable));
	}

	~Session()
	{
		std::cout << "Session has been closed.\n - Client: " << mPeerAddress.toString() << "\n\n";
		mReactor.removeEventHandler(mSocket, ReadableObserver(*this, &Session::onReadable));
	}

	void onReadable(Poco::Net::ReadableNotification* notification)
	{
		notification->release();

		try
		{
			char buffer[256] = { 0, };
			int bytesReceived = mSocket.receiveBytes(buffer, sizeof(buffer) - 1);

			if (bytesReceived > 0)
			{
				std::cout << "From Client: " << buffer << "\n\n";

				int length = strnlen_s(buffer, sizeof(buffer) - 1);
				mSocket.sendBytes(buffer, length);
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
	std::cout << "Reactor Tcp Server Framework\n\n";

	Poco::Net::SocketAddress address("127.0.0.1", 32452);
	Poco::Net::ServerSocket serverSocket(address);

	Poco::Net::SocketReactor reactor;
	Poco::Net::SocketAcceptor<Session> acceptor(serverSocket, reactor);

	reactor.run();

	return 0;
}
