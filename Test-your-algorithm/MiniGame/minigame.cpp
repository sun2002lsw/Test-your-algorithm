#include "minigame.h"

/*
	insert new user;
*/
void MiniGame::InsertUser(const UserUID& user)
{
	std::lock_guard<std::mutex> lock(mutex_);
	userSet_.insert(user);
}

/*
	game play tick
*/
void MiniGame::ProcessInputOutput()
{
	std::lock_guard<std::mutex> lock(mutex_);

	// handle input
	for (auto& inputPacket : inputPacket_)
	{
		const UserUID user = inputPacket.first;
		if (userSet_.find(user) == userSet_.end())
			continue;

		std::queue<Packet>& packetQueue = inputPacket.second;
		while (packetQueue.size() > 0)
		{
			Packet& packet = packetQueue.front();
			if (packet)
				HandleUserInput(user, packet);

			packetQueue.pop();
		}
	}

	// play game
	PlayGame();

	// update user current status
	Packet output;
	for (const auto& user : userSet_)
		if (UserStatusUpdated(user, output))
			outputPacket_[user] = output;
}

/*
	push just one input packet for one user
*/
void MiniGame::PushUserInputPacket(const UserUID& user, Packet& packet)
{
	std::lock_guard<std::mutex> lock(mutex_);
	inputPacket_[user].push(packet);
}

/*
	Get just one output packet for each user
*/
std::unordered_map<MiniGame::UserUID, Packet> MiniGame::PopUserOutputPacket()
{
	std::lock_guard<std::mutex> lock(mutex_);

	// get valid output packet
	std::unordered_map<UserUID, Packet> output;
	for (auto& outputPacket : outputPacket_)
	{
		const UserUID user = outputPacket.first;
		Packet& packet = outputPacket.second;

		if (packet)
			output[user] = packet;
	}

	// clear output packet buffer
	outputPacket_ = decltype(outputPacket_)();

	return output;
}
