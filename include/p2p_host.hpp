#ifndef __P2P_HOST_H_INCLUDED__
#define __P2P_HOST_H_INCLUDED__

#include <string>
#include <vector>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace P2PNetwork
{
	class p2p_host
	{
	public:
		std::string Ip;
		int Port;

		p2p_host();
		~p2p_host();

		std::vector<p2p_host> LoadAll(std::string filePath, 
									  std::vector<std::string> dnsSeeds, 
									  int defaultPort);

	};
}

#endif