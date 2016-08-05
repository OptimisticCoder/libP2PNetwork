#ifndef __P2P_MANAGER_H_INCLUDED__
#define __P2P_MANAGER_H_INCLUDED__

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp> 
#include <boost/signals2/signal.hpp>
#include <boost/bind.hpp>
#include <boost/uuid/uuid.hpp>            
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>        
#include <string>
#include <vector>
#include <iostream>
#include "p2p_listener.hpp"
#include "p2p_host.hpp"
#include "p2p_packet.hpp"

namespace P2PNetwork
{
	class p2p_manager
	{
	public:

		boost::signals2::signal<void(std::string)>							Log;
		boost::signals2::signal<void(bool, p2p_connection::pointer)>		NewConnection;
		boost::signals2::signal<void(p2p_connection::pointer, p2p_packet)>	DataReceived;

		p2p_manager();
		~p2p_manager();

		void Run(int incomingPort);

	private:

		void listener_run(int incomingPort);
		void outgoing_run(std::vector<p2p_host> hosts);

		void on_new_connection(bool isIncoming, p2p_connection::pointer connection);
		void on_log_recieved(std::string msg);
		void on_data_recieved(p2p_connection::pointer connection, p2p_packet packet);

		std::vector<boost::thread*> _threads;	
		p2p_listener* _listener;
		boost::uuids::uuid _networkId;
	};
}

#endif