#include "dodgeMissile.h"
#include <chrono>
#define CurrentClock std::chrono::high_resolution_clock::now()

static constexpr double AIRCRAFT_SPEED = 20;
static constexpr double MISSILE_MAX_SPEED = 25;
static constexpr double MISSILE_MIN_SPEED = 15;

/*
	create battleField, missiles, aircraft
*/
void MiniGame_DodgeMissile::Setup(Packet& packet)
{
	// create battleField
	packet >> battleField_.width_ >> battleField_.height_;

	// create missiles
	short missileCnt;
	packet >> missileCnt;
	for (short i = 0; i < missileCnt; i++)
		CreateMissileAtBattleField();

	// create aircraft
	aircraft_.Setup(battleField_);
}

bool MiniGame_DodgeMissile::IsFinish()
{
	return aircraft_.IsCrashed();
}

/*
	move aircraft & missile
*/
void MiniGame_DodgeMissile::PlayGame()
{
	auto missile = MissileList_.begin();
	size_t deletedMissileCnt = 0;

	// move missile
	while (missile != MissileList_.end())
	{
		if (missile->Move() == false)
			continue;

		if (missile->IsCrashWithObject(aircraft_))
			aircraft_.SetCrashed();

		if (missile->IsOutOfBattleField(battleField_))
		{
			missile = MissileList_.erase(missile);
			deletedMissileCnt++;
		}
		else
			missile++;
	}

	// keep the missile total count
	for (size_t i = 0; i < deletedMissileCnt; i++)
		CreateMissileAtBattleField();

	if (aircraft_.IsCrashed())
		return;

	// move aircraft
	if (aircraft_.Move())
		aircraft_.Stop();
	if (aircraft_.IsOutOfBattleField(battleField_))
		aircraft_.SetCrashed();
}

/*
	player aircraft control
*/
void MiniGame_DodgeMissile::HandleUserInput(const UserUID& user, Packet& packet)
{
	// ¡æ, ¢Ù, ¡é, ¢×, ¡ç, ¢Ø, ¡è, ¢Ö
	short direction;
	packet >> direction;
	if (direction == -1)
		return;

	const double degree = 45 * direction;
	const double radian = DegreeToRadian(degree);

	aircraft_.SetClock(CurrentClock);
	aircraft_.SetSpeed(AIRCRAFT_SPEED);
	aircraft_.SetDirection(radian);
}

/*
	update battleField state
*/
bool MiniGame_DodgeMissile::UserStatusUpdated(const UserUID& user, Packet& packet)
{
	// aircraft
	packet << aircraft_.IsCrashed() << aircraft_.GetCoordinate_X() << aircraft_.GetCoordinate_Y();
	
	// missile
	packet << MissileList_.size();
	for (const auto& m : MissileList_)
		packet << m.GetCoordinate_X() << m.GetCoordinate_Y() << m.GetDirection() << m.GetSpeed();

	// state is changed
	return true;
}

void MiniGame_DodgeMissile::CreateMissileAtBattleField()
{
	Missile missile;
	missile.Setup(battleField_);
	MissileList_.emplace_back(std::move(missile));
}

/*
	change coordinate by direction & speed
*/
bool MiniGame_DodgeMissile::FlyingObject::Move()
{
	if (speed_ == 0)
		return false;

	const auto changedClock = CurrentClock - lastClock_;
	const auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(changedClock).count();
	if (millisec < 50)
		return false;

	const double moveDistance = speed_ * static_cast<double>(millisec) / 1000.0;

	curX_ += moveDistance * cos(radianDir_);
	curY_ += moveDistance * sin(radianDir_);

	lastClock_ = CurrentClock;
	return true;
}

bool MiniGame_DodgeMissile::FlyingObject::IsCrashWithObject(const FlyingObject& obj)
{
	static constexpr double aircraftWingSize = 1;

	bool xCrash = abs(curX_ - obj.curX_) < aircraftWingSize;
	bool yCrash = abs(curY_ - obj.curY_) < aircraftWingSize;

	return xCrash && yCrash;
}

bool MiniGame_DodgeMissile::FlyingObject::IsOutOfBattleField(const BattleField& battleField)
{
	if (curX_ < 0 || curY_ < 0)
		return true;

	if (curX_ > battleField.width_ || curY_ > battleField.height_)
		return true;

	return false;
}

/*
	create missile at boundary of battleField with random speed & direction
*/
void MiniGame_DodgeMissile::Missile::Setup(const BattleField& battleField)
{
	speed_ = GetRandDouble(MISSILE_MIN_SPEED, MISSILE_MAX_SPEED);

	switch (GetRandInt(0, 3))
	{
	case 0:
		StartAtTop(battleField);
		break;
	case 1:
		StartAtBottom(battleField);
		break;
	case 2:
		StartAtLeft(battleField);
		break;
	case 3:
		StartAtRight(battleField);
		break;
	default:
		StartAtCenter(battleField);
		break;
	}

	lastClock_ = CurrentClock;
}

void MiniGame_DodgeMissile::Missile::StartAtTop(const BattleField& battleField)
{
	curX_ = GetRandDouble(0, battleField.width_);
	curY_ = 0;

	radianDir_ = GetRandDouble(DegreeToRadian(0), DegreeToRadian(180));
}

void MiniGame_DodgeMissile::Missile::StartAtBottom(const BattleField& battleField)
{
	curX_ = GetRandDouble(0, battleField.width_);
	curY_ = battleField.height_;

	radianDir_ = GetRandDouble(DegreeToRadian(180), DegreeToRadian(360));
}

void MiniGame_DodgeMissile::Missile::StartAtLeft(const BattleField& battleField)
{
	curX_ = 0;
	curY_ = GetRandDouble(0, battleField.height_);

	radianDir_ = GetRandDouble(DegreeToRadian(270), DegreeToRadian(450));
}

void MiniGame_DodgeMissile::Missile::StartAtRight(const BattleField& battleField)
{
	curX_ = battleField.width_;
	curY_ = GetRandDouble(0, battleField.height_);

	radianDir_ = GetRandDouble(DegreeToRadian(90), DegreeToRadian(270));
}

void MiniGame_DodgeMissile::Missile::StartAtCenter(const BattleField& battleField)
{
	curX_ = battleField.width_  / 2;
	curY_ = battleField.height_ / 2;

	radianDir_ = GetRandDouble(DegreeToRadian(0), DegreeToRadian(360));
}

/*
	create aircraft at center of the battleField
*/
void MiniGame_DodgeMissile::Aircraft::Setup(const BattleField & battleField)
{
	curX_ = battleField.width_  / 2;
	curY_ = battleField.height_ / 2;

	speed_ = radianDir_ = 0;
}
