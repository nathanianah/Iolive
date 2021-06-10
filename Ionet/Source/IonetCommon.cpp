#include <Ionet/IonetCommon.hpp>
#include <Ionet/olcPGEX_Network.h>

namespace ionet {
	olc::net::message<IonetMessageHeader> IonetMessagePing::Populate()
	{
		olc::net::message<IonetMessageHeader> msg;
		msg.header.id = IonetMessageHeader::PING;
		msg << time;
		return msg;
	}

	void IonetMessagePing::Unload(olc::net::message<IonetMessageHeader> input)
	{
		if (input.header.id != IonetMessageHeader::PING)
		{
			// TODO: MY BRAIN DOESN"T WORK, reword this.
			throw std::invalid_argument("Tried to unload wrong header.");
		}
		input >> time;
	}

	olc::net::message<IonetMessageHeader> IonetMessageJoinRoom::Populate()
	{
		olc::net::message<IonetMessageHeader> msg;
		msg.header.id = IonetMessageHeader::JOIN_ROOM;
		msg << room_id;
		return msg;
	}
	void IonetMessageJoinRoom::Unload(olc::net::message<IonetMessageHeader> input)
	{
		if (input.header.id != IonetMessageHeader::JOIN_ROOM)
		{
			// TODO: MY BRAIN DOESN"T WORK, reword this.
			throw std::invalid_argument("Tried to unload wrong header.");
		}
		input >> room_id;
	}

	olc::net::message<IonetMessageHeader> IonetMessageJoinRoomAccept::Populate()
	{
		olc::net::message<IonetMessageHeader> msg;
		msg.header.id = IonetMessageHeader::JOIN_ROOM_ACCEPTED;
		msg << room_id;
		return msg;
	}

	void IonetMessageJoinRoomAccept::Unload(olc::net::message<IonetMessageHeader> input)
	{
		if (input.header.id != IonetMessageHeader::JOIN_ROOM_ACCEPTED)
		{
			// TODO: MY BRAIN DOESN"T WORK, reword this.
			throw std::invalid_argument("Tried to unload wrong header.");
		}
		input >> room_id;
	}

	olc::net::message<IonetMessageHeader> IonetMessageJoinRoomReject::Populate()
	{
		olc::net::message<IonetMessageHeader> msg;
		msg.header.id = IonetMessageHeader::JOIN_ROOM_REJECTED;
		msg << room_id;
		return msg;
	}

	void IonetMessageJoinRoomReject::Unload(olc::net::message<IonetMessageHeader> input)
	{
		if (input.header.id != IonetMessageHeader::JOIN_ROOM_REJECTED)
		{
			// TODO: MY BRAIN DOESN"T WORK, reword this.
			throw std::invalid_argument("Tried to unload wrong header.");
		}
		input >> room_id;
	}

	olc::net::message<IonetMessageHeader> IonetMessageListRooms::Populate()
	{
		olc::net::message<IonetMessageHeader> msg;
		msg.header.id = IonetMessageHeader::LIST_ROOMS;
		return msg;
	}
	//Do we need unload?
	void IonetMessageListRooms::Unload(olc::net::message<IonetMessageHeader> input)
	{
		if (input.header.id != IonetMessageHeader::LIST_ROOMS)
		{
			// TODO: MY BRAIN DOESN"T WORK, reword this.
			throw std::invalid_argument("Tried to unload wrong header.");
		}
	}

	olc::net::message<IonetMessageHeader> IonetMessageSendRoom::Populate()
	{
		olc::net::message<IonetMessageHeader> msg;
		msg.header.id = IonetMessageHeader::SEND_ROOM;
		msg << room_id;
		return msg;
	}

	void IonetMessageSendRoom::Unload(olc::net::message<IonetMessageHeader> input)
	{
		if (input.header.id != IonetMessageHeader::SEND_ROOM)
		{
			// TODO: MY BRAIN DOESN"T WORK, reword this.
			throw std::invalid_argument("Tried to unload wrong header.");
		}
		input >> room_id;
	}
}