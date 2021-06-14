#pragma once
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

namespace ionet
{
	class RoomManager
	{
	public:
		bool JoinRoom(uint32_t client_id, uint32_t room_id);
		void LeaveRoom(uint32_t client_id);

		uint32_t GetRoom(uint32_t client_id);
		std::unordered_set<uint32_t> GetUsers(uint32_t room_id);
		std::unordered_set<uint32_t> GetAllRoomIds();

	private:
        // Map room ID to a list of client IDs.
        std::unordered_map<uint32_t, std::unordered_set<uint32_t>> m_rooms;
        // Map client ID to current room.
        std::unordered_map<uint32_t, uint32_t> m_user_room;
	};
}