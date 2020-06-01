#include "minigameFactory.h"
#include "protocol.h"
#include "minigameList.h"

//#define REGISTER_MINI_GAME(gameName) \ 
	//MiniGameRegister<MiniGameNumber::##gameName, MiniGame_##gameName>()

void MiniGameFactory::Setup()
{
	MiniGameRegister<MiniGameNumber::Blackjack, MiniGame_Blackjack>();
}

void MiniGameFactory::RegisterGame(const int gameNumber, std::function<MiniGame* ()> instanceCreateFunc)
{
	gameCreator_[gameNumber] = instanceCreateFunc;
}

MiniGame* MiniGameFactory::CreateGameInstance(const int gameNumber)
{
	return gameCreator_[gameNumber]();
}
