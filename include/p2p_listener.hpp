#ifndef __P2P_LISTENER_H_INCLUDED__
#define __P2P_LISTENER_H_INCLUDED__

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp> 
#include <string>
#include <algorithm>
#include <vector>
#include <boost/signals2/signal.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "p2p_connection.hpp"

using namespace boost::asio::ip;

namespace P2PNetwork
{
	class p2p_listener
	{
	public:
		p2p_listener(int incomingPort);
		~p2p_listener();

		boost::signals2::signal<void(int)>    NewConnection;

	private:
		void listenForIncoming();

		void handle_accept(p2p_connection::pointer new_connection, const boost::system::error_code& error);

		std::vector<boost::thread*> _listenerThreads;
		boost::asio::io_service _io_service;
		tcp::acceptor acceptor_;
	};
}

#endif