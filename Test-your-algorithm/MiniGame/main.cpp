#include "minigameFactory.h"
#include "protocol.h"
#include <thread>
#include <iostream>
#include <conio.h>
#include <Windows.h>

constexpr unsigned int TEST_USER_ID = 777;

// set console cursor coordinate and print char
void PrintAt(int x, int y, const std::string& c)
{
	COORD pos;

	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	
	std::cout << c;

	pos.X = 0;
	pos.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// →, ↘, ↓, ↙, ←, ↖, ↑, ↗
std::string GetArrowByDegree(double degree)
{
	int dir = (static_cast<int>(degree + 22.5) / 45) % 8;
	switch (dir)
	{
	case 0:
		return "→";
	case 1:
		return "↘";
	case 2:
		return "↓";
	case 3:
		return "↙";
	case 4:
		return "←";
	case 5:
		return "↖";
	case 6:
		return "↑";
	case 7:
		return "↗";
	default:
		return "x";
	}
}

// →, ↘, ↓, ↙, ←, ↖, ↑, ↗
short GetDirectionByKeyBoardInput()
{
	constexpr auto KEY_PUSH = 0x8000 | 0x8001;

	if (GetAsyncKeyState(VK_RIGHT) & KEY_PUSH)
		if (GetAsyncKeyState(VK_DOWN) & KEY_PUSH)
			return 1;
		else if (GetAsyncKeyState(VK_UP) & KEY_PUSH)
			return 7;
		else
			return 0;

	if (GetAsyncKeyState(VK_DOWN) & KEY_PUSH)
		if (GetAsyncKeyState(VK_LEFT) & KEY_PUSH)
			return 3;
		else
			return 2;

	if (GetAsyncKeyState(VK_LEFT) & KEY_PUSH)
		if (GetAsyncKeyState(VK_UP) & KEY_PUSH)
			return 5;
		else
			return 4;

	if (GetAsyncKeyState(VK_UP) & KEY_PUSH)
		return 6;

	// no key input
	return -1;
}

// test dodgeMissile mini-game
void TestDodgeMissile()
{
	// create client setup packet
	constexpr short width = 100;
	constexpr short height = 25;
	constexpr short missileCnt = 1;

	Packet setupPkt;
	setupPkt << width << height << missileCnt;

	// create mini-game
	MiniGameFactory::GetInstance()->Setup();
	MiniGame* minigame = MiniGameFactory::GetInstance()->CreateGameInstance(MiniGameNumber::DodgeMissile);

	// setup game
	minigame->Setup(setupPkt);
	minigame->InsertUser(TEST_USER_ID);
	
	// execute game
	std::function<void()> execute = [minigame]() {
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			minigame->ProcessInputOutput();
		}
	};

	// display game
	std::function<void()> display = [minigame, width, height]() {
		Packet outputPkt;

		bool isAircraftCrashed;
		double aircraftX, aircraftY;

		size_t missileCnt;
		double missileX, missileY, direction;

		int x, y;
		std::string arrow;
		std::queue<std::pair<int, int>> objectList;

		// draw map
		system("cls");
		for (int x = 0; x <= width + 1; x++)
			PrintAt(x, height + 1, "■");
		for (int y = 0; y <= height + 1; y++)
			PrintAt(width + 1, y, "■");

		// draw object
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50));

			// get output status
			outputPkt = minigame->PopUserOutputPacket()[TEST_USER_ID];
			if (!outputPkt)
				continue;

			// clear screen before drawing
			for (; objectList.size() > 0; objectList.pop())
			{
				x = objectList.front().first;
				y = objectList.front().second;

				PrintAt(x, y, " ");
			}

			// aircraft info
			outputPkt >> isAircraftCrashed >> aircraftX >> aircraftY;
			x = static_cast<int>(round(aircraftX));
			y = static_cast<int>(round(aircraftY));

			// draw aircraft
			if (isAircraftCrashed == false)
			{
				PrintAt(x, y, "*");
				objectList.push(std::make_pair(x, y));
			}

			// missile info
			outputPkt >> missileCnt;
			for (size_t i = 0; i < missileCnt; i++)
			{
				outputPkt >> missileX >> missileY;
				x = static_cast<int>(round(missileX));
				y = static_cast<int>(round(missileY));

				outputPkt >> direction;
				arrow = GetArrowByDegree(RadianToDegree(direction));

				PrintAt(x, y, arrow);
				objectList.push(std::make_pair(x, y));
			}
		}
	};

	// control game
	std::function<void()> control = [minigame]() {
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			short keyInput = GetDirectionByKeyBoardInput();
			if (keyInput == -1)
				continue;

			Packet inputPkt;
			inputPkt << keyInput;
			minigame->PushUserInputPacket(TEST_USER_ID, inputPkt);
		}
	};

	// test dodgeMissile
	std::thread t1(execute);
	std::thread t2(display);
	std::thread t3(control);

	t1.join();
	t2.join();
	t3.join();
}

int main()
{
	TestDodgeMissile();

	return 0;
}
