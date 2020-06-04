#pragma once
#include <corecrt_math_defines.h>
#include <type_traits>
#include <string>
#include <random>

/*
	std::is_string does not seem to exist
	so we made a handcrafted one
*/
template <typename T>
struct is_string : std::false_type {};

template <>
struct is_string<std::string> : std::true_type {};

/*
	Check if the class is iterable with for loop as in vector or list
	with existence of begin() and end()
 */
template <typename T, typename NotUsed = void>
struct is_iterable
	: std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>
	: std::true_type {};

template <typename T>
constexpr bool is_iterable_v = is_iterable<T>::value;

/*
	derived class instance creator for factory design pattern
*/
template<typename BaseClass, typename DerivedClass>
struct InstanceCreator
{
	static BaseClass* CreateInstance() { return new DerivedClass; }
};

/*
	random number generator
*/
int GetRandInt(int min, int max);
double GetRandDouble(double min, double max);

/*
	degree & radian
*/
double DegreeToRadian(const double degree);
double RadianToDegree(const double radian);
