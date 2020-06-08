#pragma once
#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
#include "utils.h"
#include "packet.h"
#include <queue>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

/*
	mini-game framework
*/
class MiniGame
{
public:
	using UserUID = unsigned int;

public:
	MiniGame() = default;
	virtual ~MiniGame() = default;

	void InsertUser(const UserUID& user);
	void ProcessInputOutput();
	void PushUserInputPacket(const UserUID& user, Packet& packet);
	std::unordered_map<UserUID, Packet> PopUserOutputPacket();

	virtual void Setup(Packet& packet) = 0;
	virtual bool IsFinish() = 0;

protected:
	virtual void PlayGame() = 0;
	virtual void HandleUserInput(const UserUID& user, Packet& packet) = 0;
	virtual bool UserStatusUpdated(const UserUID& user, Packet& packet) = 0;

	std::unordered_set<UserUID> userSet_;

private:
	std::mutex mutex_;
	std::unordered_map<UserUID, std::queue<Packet>> inputPacket_;
	std::unordered_map<UserUID, Packet> outputPacket_;
};
