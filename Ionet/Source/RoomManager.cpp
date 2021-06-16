#include <Ionet/RoomManager.hpp>
#include <Ionet/IonetCommon.hpp>
#include <stdexcept>

namespace ionet
{
    bool RoomManager::JoinRoom(uint32_t client_id, RoomId room_id)
    {
        LeaveRoom(client_id);

        bool change = m_rooms[room_id].insert(client_id).second;
        m_user_room[client_id] = room_id;
        return change;
    }

    void RoomManager::LeaveRoom(uint32_t client_id)
    {
        // Remove client_id from room.
        auto it = m_user_room.find(client_id);
        if (it != m_user_room.end())
        {
            RoomId room_id = it->second;
            auto room_it = m_rooms.find(room_id);
            if (room_it != m_rooms.end())
            {
                room_it->second.erase(client_id);
                if (room_it->second.empty())
                {
                    m_rooms.erase(room_id);
                }
            }
        }
        
        // Remove room for user.
        m_user_room.erase(client_id);
    }

    RoomId RoomManager::GetRoom(uint32_t client_id)
    {
        auto it = m_user_room.find(client_id);
        if (it == m_user_room.end())
        {
            throw std::invalid_argument("User does not belong to a room");
        }
        return it->second;
    }

    std::unordered_set<RoomId> RoomManager::GetAllRoomIds()
    {
        std::unordered_set<RoomId> room_ids;
        for (const auto& [room_id, clients] : m_rooms) {
            room_ids.insert(room_id);
        }
        return room_ids;
    }

    std::unordered_set<uint32_t> RoomManager::GetUsers(RoomId room_id)
    {
        auto it = m_rooms.find(room_id);
        if (it == m_rooms.end())
        {
            throw std::invalid_argument("Room does not exist");
        }
        return it->second;
    }
}