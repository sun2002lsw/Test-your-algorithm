#pragma once
#include "minigame.h"

class MiniGame_Blackjack final : public MiniGame, public InstanceCreator<MiniGame, MiniGame_Blackjack>
{
	virtual void Setup(Packet& packet) override;
	virtual bool IsFinish() override;
	virtual void PlayGame() override;
	virtual void HandleUserInput(const UserUID& user, Packet& packet) override;
	virtual bool UserStatusUpdated(const UserUID& user, Packet& packet) override;

	// blackjack player info
	struct PlayerInfo
	{
		// current card, current money, ...
	};

	std::vector<unsigned short> cardDeck_;
	std::unordered_map<UserUID, PlayerInfo> userPlayInfo_;
};
