#pragma once
#include "minigame.h"
#include <chrono>

class MiniGame_DodgeMissile final : public MiniGame, public InstanceCreator<MiniGame, MiniGame_DodgeMissile>
{
public:
	virtual void Setup(Packet& packet) override;
	virtual bool IsFinish() override;
	virtual void PlayGame() override;
	virtual void HandleUserInput(const UserUID& user, Packet& packet) override;
	virtual bool UserStatusUpdated(const UserUID& user, Packet& packet) override;

private:
	struct BattleField
	{
		unsigned short height_, width_;
	};

	class FlyingObject
	{
	public:
		void Move();
		bool IsCrashWithObject(const FlyingObject& obj);
		bool IsOutOfBattleField(const BattleField& battleField);

		double GetSpeed()		 const { return speed_; }
		double GetDirection()	 const { return radianDir_; }
		double GetCoordinate_X() const { return curX_; }
		double GetCoordinate_Y() const { return curY_; }

	protected:
		double speed_, radianDir_, curX_, curY_;
		std::chrono::high_resolution_clock::time_point lastClock_;
	};

	class Missile : public FlyingObject 
	{
	public:
		void Setup(const BattleField& battleField);

	private:
		void StartAtTop(const BattleField& battleField);
		void StartAtBottom(const BattleField& battleField);
		void StartAtLeft(const BattleField& battleField);
		void StartAtRight(const BattleField& battleField);
		void StartAtCenter(const BattleField& battleField);
	};

	class Aircraft : public FlyingObject
	{ 
	public:
		void Setup(const BattleField& battleField);
		void SetSpeed(const double speed) { speed_ = speed; }
		void SetDirection(const double radian) { radianDir_ = radian; }
		void SetCrashed() {	isCrashed_ = true; }
		bool IsCrashed() { return isCrashed_; }

	private:
		std::atomic<bool> isCrashed_ = false; 
	};

private:
	void CreateMissileAtBattleField();

	BattleField battleField_;
	std::list<Missile> MissileList_;
	Aircraft aircraft_;
};
