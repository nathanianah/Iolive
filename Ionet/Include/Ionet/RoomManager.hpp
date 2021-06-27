#pragma once
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

#include <Ionet/IonetCommon.hpp>

namespace ionet
{
    class RoomManager
    {
    public:
        bool JoinRoom(uint32_t client_id, uint32_t room_id);
        void LeaveRoom(uint32_t client_id);

        RoomId GetRoom(uint32_t client_id);
        std::unordered_set<RoomId> GetUsers(RoomId room_id);
        std::unordered_set<RoomId> GetAllRoomIds();

        bool CheckOrSetRoomSender(RoomId room_id, uint32_t client_id);

    private:
        // Map room ID to a list of client IDs.
        std::unordered_map<RoomId, std::unordered_set<uint32_t>> m_rooms;
        // Map client ID to current room.
        std::unordered_map<uint32_t, RoomId> m_user_room;
        // Map room ID to client sending parameters.
        std::unordered_map<RoomId, uint32_t> m_room_senders;
    };
}