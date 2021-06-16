#include <Ionet/olcPGEX_Network.h>
#include <Ionet/IonetCommon.hpp>
#include <Ionet/RoomManager.hpp>
#include <unordered_set>

namespace ionet {
    class IonetServer : public olc::net::server_interface<IonetMessageHeader>
    {
    public:
        IonetServer(uint16_t port) : olc::net::server_interface<IonetMessageHeader>(port) {}

        void MessageRoom(const olc::net::message<IonetMessageHeader>& msg, RoomId room_id,
            std::shared_ptr<olc::net::connection<IonetMessageHeader>> pIgnoreClient = nullptr)
        {
            for (const auto& client : m_deqConnections) {
                uint32_t client_id = client->GetID();
                if (pIgnoreClient && pIgnoreClient->GetID() == client_id) {
                    continue;
                }
                try
                {
                    if (room_id == m_room_manager.GetRoom(client_id))
                    {
                        MessageClient(client, msg);
                    }
                }
                catch (std::exception e)
                {
                    // Do nothing.
                }
            }
        }

    protected:
        bool OnClientConnect(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client) override
        {
            // For now, return all.
            return true;
        }

        void OnClientValidated(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client) override
        {
            // Client passed validation check, so send them a message informing
            // them they can continue to communicate
            olc::net::message<IonetMessageHeader> msg;
            msg.header.id = IonetMessageHeader::CLIENT_ACCEPTED;
            MessageClient(client, msg);
        }

        void OnClientDisconnect(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client) override
        {
            if (client)
            {
                std::cout << "[UNGRACEFUL REMOVAL]:" << client->GetID() << "\n";
                m_room_manager.LeaveRoom(client->GetID());
            }
        }

        void OnMessage(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client, olc::net::message<IonetMessageHeader>& msg) override
        {
            switch (msg.header.id)
            {
            case IonetMessageHeader::PING:
                HandlePing(client, msg);
                break;
            case IonetMessageHeader::JOIN_ROOM:
                HandleJoinRoom(client, msg);
                break;
            case IonetMessageHeader::LIST_ROOMS:
                HandleListRooms(client, msg);
                break;
            case IonetMessageHeader::MODEL_PARAMS:
                HandleModelParams(client, msg);
                break;
            default:
                break;
            }
        }
    private:
        void HandlePing(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client, olc::net::message<IonetMessageHeader>& msg) {
            // Ping back to server.
            MessageClient(client, msg);
            std::cout << "[" << client->GetID() << "] Pinging" << std::endl << std::flush;
        }

        void HandleJoinRoom(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client, olc::net::message<IonetMessageHeader>& msg) {
            // Create room if needed, then add to room.
            IonetMessageJoinRoom msg_factory;
            msg_factory.Unload(msg);
            uint32_t client_id = client->GetID();
            bool joined = m_room_manager.JoinRoom(client_id, msg_factory.room_id);
            if (joined)
            {
                std::cout << "[" << client_id << "] Adding to room " << msg_factory.room_id << std::endl;
                IonetMessageJoinRoomAccept response_factory;
                response_factory.room_id = msg_factory.room_id;
                MessageClient(client, response_factory.Populate());

                IonetMessagePing ping_factory;
                ping_factory.time = std::chrono::system_clock::now();
                MessageRoom(ping_factory.Populate(), msg_factory.room_id);
            }
            else
            {
                // Reject because already exists.
                std::cout << "[" << client_id << "] Failed to join room " << msg_factory.room_id << std::endl;
                IonetMessageJoinRoomReject response_factory;
                response_factory.room_id = msg_factory.room_id;
                MessageClient(client, response_factory.Populate());
            }
        }

        void HandleListRooms(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client, olc::net::message<IonetMessageHeader>& msg) {
            std::cout << "[" << client->GetID() << "] Sending Room List" << std::endl;
            //Loop through rooms and send room IDs.
			IonetMessageSendRooms response_factory;
            std::unordered_set<RoomId> rooms = m_room_manager.GetAllRoomIds();
			response_factory.room_ids = std::vector<RoomId>(rooms.begin(), rooms.end());
			MessageClient(client, response_factory.Populate());
        }

        void HandleModelParams(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client, olc::net::message<IonetMessageHeader>& msg) {
            // std::cout << "[" << client->GetID() << "] Forwarding model params" << std::endl;
            try
            {
				RoomId room_id = m_room_manager.GetRoom(client->GetID());
				MessageRoom(msg, room_id, client);
            }
            catch (std::exception e)
            {
                // Could not find room for user... return message error?
            }
        }

        // Map room ID to a list of client IDs.
        RoomManager m_room_manager;
    };

}

int main() {
    ionet::IonetServer server(60000);
    server.Start();
    while (true) {
        server.Update(-1, true);
    }
    return 0;
}