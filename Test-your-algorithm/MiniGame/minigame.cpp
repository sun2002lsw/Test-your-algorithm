#include "minigame.h"

/*
	game play tick
*/
void MiniGame::ProcessInputOutput()
{
	Packet packet;

	// handle input
	for (auto& userPacketQueue : inputPacket_)
	{
		const UserUID user = userPacketQueue.first;
		PacketQueue& packetQueue = userPacketQueue.second;

		if (packetQueue.try_pop(packet))
			HandleUserInput(user, packet);
	}

	// play game
	PlayGame();

	// get output
	for (const auto& user : userSet_)
		if (UserStatusUpdated(user, packet))
			outputPacket_[user].push(packet);
}

/*
	push just one input packet for one user
*/
void MiniGame::PushUserInputPacket(const UserUID& user, Packet& packet)
{
	inputPacket_[user].push(packet);
}

/*
	Get just one output packet for each user
*/
const MiniGame::UserOutputPacket MiniGame::PopUserOutputPacket()
{
	UserOutputPacket outputPacket;

	for (auto& userOutputPacket : outputPacket_)
	{
		const UserUID& user = userOutputPacket.first;
		PacketQueue& packetQueue = userOutputPacket.second;

		Packet packet;
		if (packetQueue.try_pop(packet))
			outputPacket[user] = packet;
	}

	return outputPacket;
}
