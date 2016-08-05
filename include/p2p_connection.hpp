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

		boost::signals2::signal<void(std::string)>							Log;
		boost::signals2::signal<void(bool, p2p_connection::pointer)>		NewConnection;
		tcp::socket& Socket();

		void Start();
		void Connect(std::string host, int port);

	private:
		enum { header_length = 4 };
		enum { max_body_length = 512 };

		p2p_connection(boost::asio::io_service& io_service);

		// callback declarations
		void handle_read_header(const boost::system::error_code& error);
		void handle_read_body(const boost::system::error_code& error);
		bool decode_header();

		tcp::socket socket_;
		boost::asio::io_service &_io_service;
		char data_[header_length + max_body_length];
		size_t body_length_;
	};
}

#endif