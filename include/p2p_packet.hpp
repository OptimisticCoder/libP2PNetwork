#ifndef __P2P_PACKET_H_INCLUDED__
#define __P2P_PACKET_H_INCLUDED__

#include <string>
#include <vector>
#include <iostream>

namespace P2PNetwork
{
	class p2p_packet
	{
	public:
		enum { header_length = 4 };
		enum { max_body_length = 512 };

		p2p_packet();
		~p2p_packet();

		char* data();

		const char* body() const;
		char* body();

		size_t body_length() const;
		void body_length(size_t new_length);

		bool decode_header();
	private:
		char data_[header_length + max_body_length];
		size_t body_length_;
	};
}

#endif