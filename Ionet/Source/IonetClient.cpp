#include "Ionet/IonetClient.hpp"
#include <Ionet/IonetCommon.hpp>
#include <iostream>

namespace ionet {
    IonetClient::IonetClient()
    {
        // TODO: Have the gui not access the client.

        // TODO: Names for clients???
        // TODO: Get room info (people in room) - Create messages.
        // TODO: Make rooms strings and not ints.

        // TODO: Start/stop model transmission;

        // TODO: Send all parameters, not just defaults
    }

    void IonetClient::Update()
    {
        bool has_model_params = false;
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
                if (!has_model_params)
                {
					has_model_params = true;
					HandleModelParams(msg);
                    m_param_buffer = {};
                }
                else
                {
                    m_param_buffer.push(msg);
                }
            }
            break;
            case ionet::IonetMessageHeader::LEAVE_ROOM:
                HandleLeaveRoom(msg);
            }
        }
        if (!has_model_params && !m_param_buffer.empty())
        {
            HandleModelParams(m_param_buffer.front());
            m_param_buffer.pop();
        }
    }

    void IonetClient::HandleAcceptClient(IonetMessage msg)
    {
        std::cout << "Server accepted connection." << std::endl;
        // RequestRooms();
        if (m_client_accept_handler)
        {
            m_client_accept_handler();
        }
    }

    void IonetClient::HandlePing(IonetMessage msg)
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

    void IonetClient::HandleJoinRoomAccepted(IonetMessage msg)
    {
        ionet::IonetMessageJoinRoomAccept msg_factory;
        msg_factory.Unload(msg);
        std::cout << "Successfully joined room " << msg_factory.room_id << std::endl;

        if (m_join_room_accepted_handler)
        {
            m_join_room_accepted_handler(msg_factory.room_id);
        }
    }

    void IonetClient::HandleJoinRoomRejected(IonetMessage msg)
    {
        ionet::IonetMessageJoinRoomReject msg_factory;
        msg_factory.Unload(msg);
        std::cout << "Failed to join room " << msg_factory.room_id << std::endl;

        if (m_join_room_rejected_handler)
        {
            m_join_room_rejected_handler(msg_factory.room_id);
        }
    }

    void IonetClient::HandleSendRooms(IonetMessage msg)
    {
        ionet::IonetMessageSendRooms msg_factory;
        msg_factory.Unload(msg);
        for (const RoomId& room_id : msg_factory.room_ids)
        {
            std::cout << "Obtained room " << room_id << std::endl;
        }

        if (m_send_rooms_handler)
        {
            m_send_rooms_handler(msg_factory.room_ids);
        }
    }

    void IonetClient::HandleModelParams(IonetMessage msg)
    {
        ionet::IonetMessageModelParams msg_factory;
        msg_factory.Unload(msg);
        if (m_model_params_handler)
        {
            m_model_params_handler(msg_factory.model_params);
        }
    }

    void IonetClient::HandleLeaveRoom(IonetMessage msg)
    {
        ionet::IonetMessageLeaveRoom msg_factory;
        msg_factory.Unload(msg);
        if (m_leave_room_handler)
        {
            m_leave_room_handler();
        }
    }

    void IonetClient::Ping()
    {
        IonetMessagePing msg_factory;
        msg_factory.time = std::chrono::system_clock::now();
        Send(msg_factory.Populate());
    }

    void IonetClient::JoinRoom(RoomId room_id)
    {
        IonetMessageJoinRoom msg_factory;
        msg_factory.room_id = room_id;
        Send(msg_factory.Populate());
    }

    void IonetClient::LeaveRoom()
    {
        IonetMessageLeaveRoom msg_factory;
        Send(msg_factory.Populate());
    }

    void IonetClient::RequestRooms()
    {
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

    void IonetClient::SetLeaveRoomHandler(std::function<void()> handler)
    {
        m_leave_room_handler = handler;
    }

}
