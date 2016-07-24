#include "p2p_connection.hpp"

namespace P2PNetwork
{
	p2p_connection::p2p_connection(boost::asio::io_service& io_service) : socket_(io_service)
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
}