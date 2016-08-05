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
		// incoming cycle
		boost::asio::async_read(socket_,
			boost::asio::buffer(data_, p2p_connection::header_length),
			boost::bind(
			&p2p_connection::handle_read_header, shared_from_this(),
			boost::asio::placeholders::error));
	}

	void p2p_connection::Connect(std::string host, int port)
	{
		// outgoing cycle
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
			// there was an error connecting, so log it and exit the method, so we can try again.
			Log(ex.what());
			return;
		}

		// in theory, we're connected ...
		NewConnection(false, shared_from_this());
	}

	void p2p_connection::handle_read_header(const boost::system::error_code& error)
	{
		if (!error && decode_header())
		{
			boost::asio::async_read(socket_,
				boost::asio::buffer(data_ + header_length, body_length_),
				boost::bind(&p2p_connection::handle_read_body, shared_from_this(),
				boost::asio::placeholders::error));
		}
		else
		{
			// TODO: we got disconnected or sumfin
		}
	}

	void p2p_connection::handle_read_body(const boost::system::error_code& error)
	{

	}

	bool p2p_connection::decode_header()
	{
		char header[header_length + 1] = "";
		std::strncat(header, data_, header_length);
		body_length_ = std::atoi(header);
		if (body_length_ > max_body_length)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}
}