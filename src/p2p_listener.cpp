#include "p2p_listener.hpp"

namespace P2PNetwork
{
	p2p_listener::p2p_listener(boost::asio::io_service &io_service, int incomingPort) 
		: _io_service(io_service), acceptor_(io_service, tcp::endpoint(tcp::v4(), incomingPort))
	{
		listenForIncoming();
	}

	p2p_listener::~p2p_listener() 		
	{
	}

	void p2p_listener::listenForIncoming()
	{
		p2p_connection::pointer new_connection = p2p_connection::Create(_io_service);

		acceptor_.async_accept(new_connection->Socket(),
			boost::bind(&p2p_listener::handle_accept, this, new_connection,
			boost::asio::placeholders::error));
	}

	void p2p_listener::handle_accept(p2p_connection::pointer new_connection, const boost::system::error_code& error)
	{
		if (!error)
		{
			NewConnection(200);
			new_connection->Start();
			listenForIncoming();
		}
	}
}