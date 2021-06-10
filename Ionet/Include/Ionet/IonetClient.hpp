#pragma once
#include <Ionet/olcPGEX_Network.h>
#include <Ionet/IonetCommon.hpp>

namespace ionet {
	class IonetClient : public olc::net::client_interface<IonetMessageHeader>
	{
	public:
		IonetClient(std::string address, uint16_t port);

		void Update();
		void Ping();
		void JoinRoom(uint32_t room_id);
	};
}