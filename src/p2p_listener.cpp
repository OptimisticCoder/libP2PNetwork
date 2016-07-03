#include "p2p_listener.hpp"

namespace P2PNetwork
{
	p2p_listener::p2p_listener()
	{

	}

	p2p_listener::~p2p_listener()
	{
		_io_service.stop();

		for (std::vector<boost::thread*>::iterator it = _listenerThreads.begin(); it != _listenerThreads.end(); ++it) {
			(*it)->interrupt();
			delete (*it);
		}
		_listenerThreads.clear();
	}

	void p2p_listener::Start(int incomingPort, std::string myName, std::string publicKey)
	{
		tcp::resolver resolver(_io_service);
		tcp::resolver::query query(host_name().c_str(), "");
		for (tcp::resolver::iterator i = resolver.resolve(query); i != tcp::resolver::iterator(); ++i)
		{
			boost::thread* listenerThread = new boost::thread(boost::bind(&p2p_listener::listenForIncoming, this, *i, &_io_service));
			_listenerThreads.push_back(listenerThread);
		}
	}

	void p2p_listener::listenForIncoming(tcp::endpoint endpoint, boost::asio::io_service* io_service)
	{
		tcp::acceptor* acceptor;

		try {
			acceptor = new tcp::acceptor(*io_service);

			// Open the acceptor with the option to reuse the address
			// (i.e. SO_REUSEADDR)
			acceptor->open(endpoint.protocol());
			acceptor->set_option(
				tcp::acceptor::reuse_address(true)
				);
			acceptor->bind(endpoint);
			acceptor->listen();
		}
		catch (boost::system::system_error e) {
			std::cerr << "Error binding to " << endpoint.address().to_string() << ":" << endpoint.port() << ": " << e.what() << std::endl;
		}

		delete acceptor;
	}
}