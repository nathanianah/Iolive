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

		LEAVE_ROOM,

		REQUEST_ROOMS,
		SEND_ROOMS,
	};

	typedef olc::net::message<IonetMessageHeader> IonetMessage;

	struct IonetMessageBase
	{
		virtual olc::net::message<IonetMessageHeader> Populate() = 0;
		virtual void Unload(olc::net::message<IonetMessageHeader> input) = 0;
	};

	struct IonetMessageModelParams : IonetMessageBase
	{
		struct Parameter {
			int index;
			float value;
		};
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		std::map<int, float> model_params;
	};

	struct IonetMessagePing : IonetMessageBase
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		std::chrono::time_point<std::chrono::system_clock> time;
	};

	struct IonetMessageJoinRoom : public IonetMessageBase
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		RoomId room_id;
	};

	struct IonetMessageJoinRoomAccept : public IonetMessageBase
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		RoomId room_id;
	};

	struct IonetMessageJoinRoomReject : public IonetMessageBase
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		RoomId room_id;
	};

	struct IonetMessageLeaveRoom : public IonetMessageBase
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);
	};

	struct IonetMessageRequestRooms : public IonetMessageBase
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);
	};

	struct IonetMessageSendRooms : public IonetMessageBase
	{
		olc::net::message<IonetMessageHeader> Populate();
		void Unload(olc::net::message<IonetMessageHeader> input);

		std::vector<RoomId> room_ids;
	};
}
