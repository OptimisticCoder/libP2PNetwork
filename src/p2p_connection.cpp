#include "p2p_connection.hpp"

namespace P2PNetwork
{
	p2p_connection::p2p_connection(boost::asio::io_service& io_service) : _io_service(io_service), socket_(io_service)
	{
	}

	tcp::socket& p2p_connection::Socket()
	{
		return socket_;
	}

	void p2p_connection::Start()
	{
		// todo: start data exchange cycle
	}

	void p2p_connection::Connect(std::string host, int port)
	{
		std::stringstream ss;
		ss << port;

		tcp::resolver resolver(_io_service);
		tcp::resolver::query query(tcp::v4(), host.c_str(), ss.str());
		tcp::resolver::iterator iterator = resolver.resolve(query);

		try
		{
			boost::asio::connect(socket_, iterator);
		}
		catch (std::exception const &ex) {
			Log(ex.what());
		}
	}
}