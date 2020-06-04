#include "minigameFactory.h"
#include "protocol.h"

int main()
{
	MiniGameFactory::GetInstance()->Setup();
	MiniGame* minigame = MiniGameFactory::GetInstance()->CreateGameInstance(MiniGameNumber::DodgeMissile);

	while (true)
		minigame->ProcessInputOutput();

	return 0;
}