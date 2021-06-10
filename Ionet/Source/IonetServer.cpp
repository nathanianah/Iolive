#include <Ionet/olcPGEX_Network.h>
#include <Ionet/IonetCommon.hpp>
#include <unordered_set>

namespace ionet {
	class IonetServer : public olc::net::server_interface<IonetMessageHeader>
	{
	public:
		IonetServer(uint16_t port) : olc::net::server_interface<IonetMessageHeader>(port) {}

		void MessageRoom(const olc::net::message<IonetMessageHeader>& msg, uint32_t room_id,
			std::shared_ptr<olc::net::connection<IonetMessageHeader>> pIgnoreClient = nullptr)
		{
			for (const auto& client : m_deqConnections) {
				uint32_t client_id = client->GetID();
				if (pIgnoreClient && pIgnoreClient->GetID() == client_id) {
					continue;
				}

				auto it = m_user_room.find(client_id);
				if (it != m_user_room.end()) {
					if (it->second == room_id) {
						client->Send(msg);
					}
				}
			}
		}

	protected:
		bool OnClientConnect(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client) override
		{
			// For now, return all.
			std::cout << "Client tries to connect" << std::endl;;
			return true;
		}

		void OnClientValidated(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client) override
		{
			// Client passed validation check, so send them a message informing
			// them they can continue to communicate
			olc::net::message<IonetMessageHeader> msg;
			msg.header.id = IonetMessageHeader::CLIENT_ACCEPTED;
			// TODO: Check if SendMessage(client, msg) does the same/is safer?
			client->Send(msg);
		}

		void OnClientDisconnect(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client) override
		{
			if (client)
			{
				std::cout << "[UNGRACEFUL REMOVAL]:" << client->GetID() << "\n";
			}

		}
		void OnMessage(std::shared_ptr<olc::net::connection<IonetMessageHeader>> client, olc::net::message<IonetMessageHeader>& msg) override
		{
			switch (msg.header.id)
			{
			case IonetMessageHeader::PING:
			{
				// Ping back to server.
				client->Send(msg);
				std::cout << "[" << client->GetID() << "] Pinging" << std::endl << std::flush;
			}
			break;
			case IonetMessageHeader::JOIN_ROOM:
			{
				// Create room if needed, then add to room.
				IonetMessageJoinRoom msg_factory;
				msg_factory.Unload(msg);
				std::scoped_lock lock(mu);
				std::unordered_set<uint32_t>& users = m_rooms.insert({
						msg_factory.room_id,
						{}
					}).first->second;

				uint32_t client_id = client->GetID();
				if (users.find(client_id) == users.end()) {
					// Add user to room.
					std::cout << "[" << client_id << "] Adding to room " << msg_factory.room_id << std::endl;
					users.insert(client_id);
					m_user_room.insert_or_assign(client_id, msg_factory.room_id);
					IonetMessageJoinRoomAccept response_factory;
					response_factory.room_id = msg_factory.room_id;
					client->Send(response_factory.Populate());

					IonetMessagePing ping_factory;
					ping_factory.time = std::chrono::system_clock::now();
					MessageRoom(ping_factory.Populate(), msg_factory.room_id);
				}
				else
				{
					// Reject because already exists.
					std::cout << "[" << client_id << "] Already in room " << msg_factory.room_id << std::endl;
					IonetMessageJoinRoomReject response_factory;
					response_factory.room_id = msg_factory.room_id;
					client->Send(response_factory.Populate());
				}

			}
				break;
			default:
				break;
			}
		}
	private:
		// Map room ID to a list of client IDs.
		std::unordered_map<uint32_t, std::unordered_set<uint32_t>> m_rooms;
		std::unordered_map<uint32_t, uint32_t> m_user_room;
		std::mutex mu;
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