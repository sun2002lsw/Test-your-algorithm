#include "pch.h"
#include "utils.h"

int GetRandInt(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 mt(rd());

	std::uniform_int_distribution<> distrib(min, max);
	return distrib(mt);
}

double GetRandDouble(double min, double max)
{
	static std::random_device rd;
	static std::mt19937 mt(rd());

	std::uniform_real_distribution<> distrib(min, max);
	return distrib(mt);
}

double DegreeToRadian(const double degree)
{ 
	return degree * M_PI / 180; 
}

double RadianToDegree(const double radian)
{ 
	return radian * 180 / M_PI; 
}
