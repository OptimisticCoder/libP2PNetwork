#include "p2p_connection.hpp"

namespace P2PNetwork
{
	p2p_connection::p2p_connection(boost::asio::io_service& io_service, boost::uuids::uuid &localId) 
		: _io_service(io_service), socket_(io_service), _localId(localId)
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
			boost::asio::buffer(packet_.data(), p2p_packet::header_length),
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

		std::stringstream id_stream;
		id_stream << "IDNT" << _localId;

		Send(std::string(id_stream.str()));
	}

	void p2p_connection::Send(p2p_packet packet)
	{
		bool write_in_progress = !write_queue_.empty();
		write_queue_.push_back(packet);
		if (!write_in_progress)
		{
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_queue_.front().data(),
				write_queue_.front().length()),
				boost::bind(&p2p_connection::handle_write, shared_from_this(),
				boost::asio::placeholders::error));
		}
	}

	void p2p_connection::Send(char* data, size_t length)
	{
		packet_.body_length(length);
		memcpy(packet_.body(), data, packet_.body_length());
		packet_.encode_header();

		Send(packet_);
	}

	void p2p_connection::Send(std::string message)
	{
		packet_.body_length(strlen(message.c_str()));
		memcpy(packet_.body(), message.c_str(), packet_.body_length());
		packet_.encode_header();

		Send(packet_);
	}

	void p2p_connection::handle_read_header(const boost::system::error_code& error)
	{
		if (!error && packet_.decode_header())
		{
			boost::asio::async_read(socket_,
				boost::asio::buffer(packet_.body(), packet_.body_length()),
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
		if (!error)
		{
			std::string body(packet_.body(), packet_.body() + packet_.body_length());
			std::string typeCode = body.substr(0, 4);
			if (typeCode == "IDNT")
			{
				boost::uuids::string_generator str_gen;
				_remoteId = str_gen(body.substr(4, packet_.body_length() - 4));
				if (boost::uuids::to_string(_localId) != boost::uuids::to_string(_remoteId))
				{
					NewConnection(true, shared_from_this());

					std::stringstream id_stream;
					id_stream << "IDOK" << _localId;

					Send(std::string(id_stream.str()));
				}
				else
				{
					std::stringstream ss;
					ss << "Dropped connection to self. " << socket_.remote_endpoint();
					Log(std::string(ss.str()));

					socket_.close();
				}
			}
			else
				ReceivedData(shared_from_this(), packet_);

			//boost::asio::async_read(socket_,
			//	boost::asio::buffer(packet_.data(), p2p_packet::header_length),
			//	boost::bind(&p2p_connection::handle_read_header, shared_from_this(),
			//	boost::asio::placeholders::error));

			if (write_queue_.empty())
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
				Send(std::string("PING"));
			}
			//else
			//	boost::asio::async_write(socket_,
			//		boost::asio::buffer(write_queue_.front().data(),
			//		write_queue_.front().length()),
			//		boost::bind(&p2p_connection::handle_write, shared_from_this(),
			//		boost::asio::placeholders::error));
		}
		else
		{
			// TODO: we got disconnected or sumfin
		}
	}

	void p2p_connection::handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			write_queue_.pop_front();

			boost::asio::async_read(socket_,
				boost::asio::buffer(packet_.data(), p2p_packet::header_length),
				boost::bind(&p2p_connection::handle_read_header, shared_from_this(),
				boost::asio::placeholders::error));
		}
		else
		{
			// TODO: we got disconnected or sumfin
		}
	}
}