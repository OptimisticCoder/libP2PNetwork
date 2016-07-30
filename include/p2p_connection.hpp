#ifndef __P2P_CONNECTION_H_INCLUDED__
#define __P2P_CONNECTION_H_INCLUDED__

#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/signals2/signal.hpp>

using namespace boost::asio::ip;

namespace P2PNetwork
{
	class p2p_connection
		: public boost::enable_shared_from_this<p2p_connection>
	{
	public:
		typedef boost::shared_ptr<p2p_connection> pointer;

		static pointer Create(boost::asio::io_service& io_service)
		{
			return pointer(new p2p_connection(io_service));
		}

		boost::signals2::signal<void(std::string)>				Log;
		tcp::socket& Socket();

		void Start();

		void Connect(std::string host, int port);

	private:
		p2p_connection(boost::asio::io_service& io_service);

		// todo: callback declarations

		tcp::socket socket_;
		boost::asio::io_service &_io_service;
	};
}

#endif