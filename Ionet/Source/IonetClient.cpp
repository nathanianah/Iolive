#include "Ionet/IonetClient.hpp"
#include <Ionet/IonetCommon.hpp>
#include <iostream>

namespace ionet {
    IonetClient::IonetClient()
    {
        // TODO: Have server periodically check for stale connections.

        // TODO: Implement a bool is_ready for sending...

        // TODO: Implement one sender per room in the server.
        // TODO: Have the gui not access the client.

        // TODO: Implement into Application/MainGui.
        // TODO: Implement send/recieve parameters in Application.
        // TODO: MainGui connected to room page... list others?
        // TODO: Get room info (people in room) - Create messages.
        // TODO: Make rooms strings and not ints.

        // TODO: Names for clients???

        // TODO: Start/stop model transmission;
        // TODO: Networking on its own thread on application?
        // Might need to look into this to separate fps from network.
    }

    void IonetClient::Update()
    {
        while (!Incoming().empty())
        {
            auto msg = Incoming().pop_front().msg;
            switch (msg.header.id)
            {
            case ionet::IonetMessageHeader::CLIENT_ACCEPTED:
            {
                HandleAcceptClient(msg);
            }
            break;
            case ionet::IonetMessageHeader::PING:
            {
                HandlePing(msg);
            }
            break;
            case ionet::IonetMessageHeader::JOIN_ROOM_ACCEPTED:
            {
                HandleJoinRoomAccepted(msg);
            }
            break;
            case ionet::IonetMessageHeader::JOIN_ROOM_REJECTED:
            {
                HandleJoinRoomRejected(msg);
            }
            break;
            case ionet::IonetMessageHeader::SEND_ROOMS:
            {
                HandleSendRooms(msg);
            }
            break;
            case ionet::IonetMessageHeader::MODEL_PARAMS:
            {
                HandleModelParams(msg);
            }
            }
        }
    }

    void IonetClient::HandleAcceptClient(olc::net::message<IonetMessageHeader> msg)
    {
        std::cout << "Server accepted connection." << std::endl;
        // RequestRooms();
        if (m_client_accept_handler)
        {
            m_client_accept_handler();
        }
    }

    void IonetClient::HandlePing(olc::net::message<IonetMessageHeader> msg)
    {
        ionet::IonetMessagePing msg_factory;
        msg_factory.Unload(msg);
        auto ping = std::chrono::system_clock::now().time_since_epoch() - msg_factory.time.time_since_epoch();
        std::cout << "Pinging " << ping.count() << std::endl;

        if (m_ping_handler)
        {
            m_ping_handler(ping);
        }
    }

    void IonetClient::HandleJoinRoomAccepted(olc::net::message<IonetMessageHeader> msg)
    {
        ionet::IonetMessageJoinRoomAccept msg_factory;
        msg_factory.Unload(msg);
        std::cout << "Successfully joined room " << msg_factory.room_id << std::endl;

        if (m_join_room_accepted_handler)
        {
            m_join_room_accepted_handler(msg_factory.room_id);
        }
    }

    void IonetClient::HandleJoinRoomRejected(olc::net::message<IonetMessageHeader> msg)
    {
        ionet::IonetMessageJoinRoomReject msg_factory;
        msg_factory.Unload(msg);
        std::cout << "Failed to join room " << msg_factory.room_id << std::endl;

        if (m_join_room_rejected_handler)
        {
            m_join_room_rejected_handler(msg_factory.room_id);
        }
    }

    void IonetClient::HandleSendRooms(olc::net::message<IonetMessageHeader> msg)
    {
        ionet::IonetMessageSendRooms msg_factory;
        msg_factory.Unload(msg);
        m_rooms = msg_factory.room_ids;
        for (const RoomId& room_id : m_rooms)
        {
            std::cout << "Obtained room " << room_id << std::endl;
        }

        if (m_send_rooms_handler)
        {
            m_send_rooms_handler(msg_factory.room_ids);
        }
    }

    void IonetClient::HandleModelParams(olc::net::message<IonetMessageHeader> msg)
    {
        ionet::IonetMessageModelParams msg_factory;
        msg_factory.Unload(msg);
        if (m_model_params_handler)
        {
            m_model_params_handler(msg_factory.model_params);
        }
    }

    void IonetClient::Ping()
    {
        IonetMessagePing msg_factory;
        msg_factory.time = std::chrono::system_clock::now();
        auto msg = msg_factory.Populate();
        Send(msg);
    }

    void IonetClient::JoinRoom(RoomId room_id)
    {
        IonetMessageJoinRoom msg_factory;
        msg_factory.room_id = room_id;
        auto msg = msg_factory.Populate();
        Send(msg);
    }

    void IonetClient::LeaveRoom()
    {
        m_rooms.clear();
        IonetMessageRequestRooms msg_factory;
        Send(msg_factory.Populate());
    }

    void IonetClient::RequestRooms()
    {
        m_rooms.clear();
        IonetMessageRequestRooms msg_factory;
        Send(msg_factory.Populate());
    }

    void IonetClient::SendParams(std::map<int, float> parameters)
    {
        IonetMessageModelParams msg_factory;
        msg_factory.model_params = parameters;
        Send(msg_factory.Populate());
    }

    void IonetClient::SetClientAcceptHandler(std::function<void()> handler)
    {
        m_client_accept_handler = handler;
    }
    void IonetClient::SetPingHandler(std::function<void(std::chrono::system_clock::duration)> handler)
    {
        m_ping_handler = handler;
    }

    void IonetClient::SetJoinRoomAcceptedHandler(std::function<void(RoomId room_id)> handler)
    {
        m_join_room_accepted_handler = handler;
    }

    void IonetClient::SetJoinRoomRejectedHandler(std::function<void(RoomId room_id)> handler)
    {
        m_join_room_rejected_handler = handler;
    }

    void IonetClient::SetSendRoomsHandler(std::function<void(std::vector<RoomId> room_ids)> handler)
    {
        m_send_rooms_handler = handler;
    }

    void IonetClient::SetModelParamsHandler(std::function<void(std::map<int, float> parameters)> handler)
    {
        m_model_params_handler = handler;
    }

}
