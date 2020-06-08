#pragma once
#include "minigame.h"
#include <atomic>

class MiniGame_DodgeMissile final : public MiniGame, public InstanceCreator<MiniGame, MiniGame_DodgeMissile>
{
	using Clock = std::chrono::high_resolution_clock::time_point;

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
		bool Move();
		bool IsCrashWithObject(const FlyingObject& obj);
		bool IsOutOfBattleField(const BattleField& battleField);

		double GetSpeed()		 const { return speed_; }
		double GetDirection()	 const { return radianDir_; }
		double GetCoordinate_X() const { return curX_; }
		double GetCoordinate_Y() const { return curY_; }

	protected:
		double speed_ = 0, radianDir_ = 0, curX_ = 0, curY_ = 0;
		Clock lastClock_;
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
		void Stop() { speed_ = 0; }
		void SetClock(Clock lastClock) { lastClock_ = lastClock; }
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
