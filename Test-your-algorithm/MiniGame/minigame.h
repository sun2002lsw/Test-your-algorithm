#pragma once
#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
#include "utils.h"
#include "packet.h"
#include <unordered_map>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>
#include <concurrent_unordered_set.h>

/*
	mini-game framework
*/
class MiniGame
{
public:
	using UserUID = unsigned int;
	using PacketQueue = concurrency::concurrent_queue<Packet>;
	using UserPacketQueue = concurrency::concurrent_unordered_map<UserUID, PacketQueue>;
	using UserOutputPacket = std::unordered_map<UserUID, Packet>;
	using UserSet = concurrency::concurrent_unordered_set<UserUID>;

public:
	MiniGame() = default;
	virtual ~MiniGame() = default;

	void InsertUser(const UserUID& user) { userSet_.insert(user); }
	void ProcessInputOutput();
	void PushUserInputPacket(const UserUID& user, Packet& packet);
	const UserOutputPacket PopUserOutputPacket();

	virtual void Setup(Packet& packet) = 0;
	virtual bool IsFinish() = 0;

protected:
	virtual void PlayGame() = 0;
	virtual void HandleUserInput(const UserUID& user, Packet& packet) = 0;
	virtual bool UserStatusUpdated(const UserUID& user, Packet& packet) = 0;

	UserSet userSet_;

private:
	UserPacketQueue inputPacket_;
	UserPacketQueue outputPacket_;
};
