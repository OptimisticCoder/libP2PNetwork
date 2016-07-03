#ifndef __P2P_LISTENER_H_INCLUDED__
#define __P2P_LISTENER_H_INCLUDED__

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp> 
#include <string>
#include <algorithm>
#include <vector>

using namespace boost::asio::ip;

namespace P2PNetwork
{
	class p2p_listener
	{
	public:
		p2p_listener();
		~p2p_listener();

		void Start(int incomingPort, std::string myName, std::string publicKey);

	private:
		void listenForIncoming(boost::asio::ip::tcp::endpoint endpoint, boost::asio::io_service* port);

		std::vector<boost::thread*> _listenerThreads;
		boost::asio::io_service _io_service;
	};
}

#endif