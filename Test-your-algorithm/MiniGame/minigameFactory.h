#pragma once
#include "minigame.h"
#include "macro.h"
#include <functional>

/*
	create mini game instance by game number
*/
class MiniGameFactory
{
	THIS_CLASS_IS_SINGLETON(MiniGameFactory)

public:
	void Setup();
	void RegisterGame(const int gameNumber, std::function<MiniGame* ()> instanceCreateFunc);
	MiniGame* CreateGameInstance(const int gameNumber);

private:
	std::unordered_map<int, std::function<MiniGame* ()>> gameCreator_;
};

/*
	register "minigame instance create function" by "game number" at constructor
*/
template<int gameNumber, typename GameClass>
struct MiniGameRegister
{
	MiniGameRegister()
	{
		MiniGameFactory::GetInstance()->RegisterGame(gameNumber, &GameClass::CreateInstance);
	}
};
