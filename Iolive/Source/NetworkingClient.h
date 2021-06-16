#pragma once
#include <Ionet/IonetClient.hpp>

namespace Iolive
{
	class NetworkingClient
	{
	public:
		virtual bool IsConnected() = 0;
		virtual void RequestRooms() = 0;
		virtual void JoinRoom(ionet::RoomId room_id) = 0;
		virtual std::vector<ionet::RoomId> GetRooms() = 0;
		// void SetRoomList(std::vector<ionet::RoomId> room_ids) { m_rooms = room_ids; }

		std::vector<ionet::RoomId> m_rooms;

		bool m_in_room;
		ionet::RoomId m_current_room;
		std::map<int, float> ModelParams;
	};
}
