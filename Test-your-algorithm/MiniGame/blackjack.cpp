#include "blackjack.h"
#include <random>
#include <algorithm>

/*
	setup card deck
*/
void MiniGame_Blackjack::Setup(Packet& packet)
{
	unsigned short deckCnt;
	packet >> deckCnt;

	// A ~ 10 card
	for (unsigned short i = 0; i < deckCnt; i++)
		for (unsigned short cardNum = 1; cardNum < 11; cardNum++)
			cardDeck_.push_back(cardNum);

	// J, Q, K card
	for (unsigned short i = 0; i < deckCnt; i++)
		for (unsigned short j = 0; j < 3; j++)
			cardDeck_.push_back(10);

	// shuffle deck
	std::random_device rd;
	std::mt19937 mt(rd());
	std::shuffle(cardDeck_.begin(), cardDeck_.end(), mt);
}

/*
	how can we check?
*/
bool MiniGame_Blackjack::IsFinish()
{
	return false;
}

/*
	dealer play
	calcuate all player card sum
	determine busrt player
*/
void MiniGame_Blackjack::PlayGame()
{
}

/*
	hit, stay, split
*/
void MiniGame_Blackjack::HandleUserInput(const UserUID& user, Packet& packet)
{
}

/*
	my card, dealer card, my money
*/
bool MiniGame_Blackjack::UserStatusUpdated(const UserUID& user, Packet& packet)
{
	// if (nothing has changed)
		return false;
}
