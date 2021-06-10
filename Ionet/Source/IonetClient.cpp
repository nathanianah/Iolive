#include "Ionet/IonetClient.hpp"
#include <Ionet/IonetCommon.hpp>
#include <iostream>

namespace ionet {
	IonetClient::IonetClient(std::string address, uint16_t port)
	{
		if (Connect(address, port))
		{
			if (IsConnected()) {
				std::cout << "Connected" << std::endl;
			}
			else
			{
				std::cout << "Connection failed" << std::endl;
			}
		}
		else
		{
			std::cout << "Connection failed" << std::endl;
		}
	}

	void IonetClient::Update()	{
		if (!Incoming().empty())
		{
			auto msg = Incoming().pop_front().msg;
			switch (msg.header.id)
			{
			case ionet::IonetMessageHeader::CLIENT_ACCEPTED:
			{
				std::cout << "Server accepted connection." << std::endl;
			}
			break;
			case ionet::IonetMessageHeader::PING:
			{
				ionet::IonetMessagePing msg_factory;
				msg_factory.Unload(msg);
				auto ping = std::chrono::system_clock::now().time_since_epoch() - msg_factory.time.time_since_epoch();
				std::cout << "Pinging " << ping.count() << std::endl;
			}
			break;
			case ionet::IonetMessageHeader::JOIN_ROOM_ACCEPTED:
			{
				ionet::IonetMessageJoinRoomAccept msg_factory;
				msg_factory.Unload(msg);
				std::cout << "Successfully joined room " << msg_factory.room_id << std::endl;
			}
			break;
			case ionet::IonetMessageHeader::JOIN_ROOM_REJECTED:
			{
				ionet::IonetMessageJoinRoomReject msg_factory;
				msg_factory.Unload(msg);
				std::cout << "Already in join room " << msg_factory.room_id << std::endl;
			}
			break;
			case ionet::IonetMessageHeader::SEND_ROOM:
			{
				ionet::IonetMessageSendRoom msg_factory;
				msg_factory.Unload(msg);
				std::cout << "Obtained room " << msg_factory.room_id << std::endl;
				m_rooms.push_back(msg_factory.room_id);
			}
			break;
			}
		}
	}

	void IonetClient::Ping()
	{
		IonetMessagePing msg_factory;
		msg_factory.time = std::chrono::system_clock::now();
		auto msg = msg_factory.Populate();
		Send(msg);
	}

	void IonetClient::JoinRoom(uint32_t room_id)
	{
		IonetMessageJoinRoom msg_factory;
		msg_factory.room_id = room_id;
		auto msg = msg_factory.Populate();
		Send(msg);
	}

	void IonetClient::ListRooms()
	{
		m_rooms.clear();
		IonetMessageListRooms msg_factory;
		Send(msg_factory.Populate());
	}

	void IonetClient::PrintRooms()
	{
		std::cout << "Rooms: ";
		for (uint32_t r : m_rooms) std::cout << r << " ";
		std::cout << std::endl;
	}
}
