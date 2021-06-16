#pragma once
#include <Ionet/olcPGEX_Network.h>

#include <chrono>
#include <map>

namespace ionet {
    typedef uint32_t RoomId;

	enum class IonetMessageHeader : uint32_t
	{
		PING,
		CLIENT_ACCEPTED,
		MODEL_PARAMS,

		JOIN_ROOM,
		JOIN_ROOM_ACCEPTED,
		JOIN_ROOM_REJECTED,

		LIST_ROOMS,
		SEND_ROOMS,
	};

	struct IonetMessage
	{
		virtual olc::net::message<IonetMessageHeader> Populate() = 0;
		virtual void Unload(olc::net::message<IonetMessageHeader> input) = 0;
	};

	struct IonetMessageModelParams : IonetMessage
	{
		struct Parameter {
			int index;
			float value;
		};
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		std::map<int, float> model_params;
	};

	struct IonetMessagePing : IonetMessage
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		std::chrono::time_point<std::chrono::system_clock> time;
	};

	struct IonetMessageJoinRoom : public IonetMessage
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		RoomId room_id;
	};

	struct IonetMessageJoinRoomAccept : public IonetMessage
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		RoomId room_id;
	};

	struct IonetMessageJoinRoomReject : public IonetMessage
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		RoomId room_id;
	};

	struct IonetMessageListRooms : public IonetMessage
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);
	};

	struct IonetMessageSendRooms : public IonetMessage
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		std::vector<RoomId> room_ids;
	};
}
