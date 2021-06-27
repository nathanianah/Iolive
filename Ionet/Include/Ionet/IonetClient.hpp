#pragma once
#include <Ionet/olcPGEX_Network.h>
#include <Ionet/IonetCommon.hpp>
#include <queue>

namespace ionet {
    constexpr std::array<const char*, 2> kClientModes({
        "Send",
        "Receive",
    });

    enum class ClientMode {
        SEND,
        RECEIVE,
    };

    class IonetClient : public olc::net::client_interface<IonetMessageHeader>
    {
    public:
        IonetClient();

        void Update();
        void Ping();
        void JoinRoom(RoomId room_id);
        void LeaveRoom();
        void RequestRooms();
        virtual void SendParams(std::map<int, float> parameters);

        // Message receive handlers.
        void SetClientAcceptHandler(std::function<void()> handler);
        void SetPingHandler(std::function<void(std::chrono::system_clock::duration)> handler);
        void SetJoinRoomAcceptedHandler(std::function<void(RoomId room_id)> handler);
        void SetJoinRoomRejectedHandler(std::function<void(RoomId room_id)> handler);
        void SetSendRoomsHandler(std::function<void(std::vector<RoomId> room_ids)> handler);
        void SetModelParamsHandler(std::function<void(std::map<int, float> parameters)> handler);
        void SetLeaveRoomHandler(std::function<void()> handler);

    protected:
        void HandleAcceptClient(IonetMessage msg);
        void HandlePing(IonetMessage msg);
        void HandleJoinRoomAccepted(IonetMessage msg);
        void HandleJoinRoomRejected(IonetMessage msg);
        void HandleSendRooms(IonetMessage msg);
        void HandleModelParams(IonetMessage msg);
        void HandleLeaveRoom(IonetMessage msg);

        std::function<void()> m_client_accept_handler;
        std::function<void(std::chrono::system_clock::duration ping)> m_ping_handler;
        std::function<void(RoomId room_id)> m_join_room_accepted_handler;
        std::function<void(RoomId room_id)> m_join_room_rejected_handler;
        std::function<void(std::vector<RoomId> room_ids)> m_send_rooms_handler;
        std::function<void(std::map<int, float> parameters)> m_model_params_handler;
        std::function<void()> m_leave_room_handler;

        std::queue<IonetMessage> m_param_buffer;
    };

    class IonetSendClient : public IonetClient { };

    class IonetReceiveClient : public IonetClient
    {
        void SendParams(std::map<int, float> parameters) {};
    };

}
