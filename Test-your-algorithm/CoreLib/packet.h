#pragma once
#include "packetBase.h"

static constexpr size_t NORMAL_SIZE =   4 * 1024;
static constexpr size_t SMALL_SIZE	=   1 * 1024;
static constexpr size_t LARGE_SIZE	=  16 * 1024;

class Packet		: public PacketBase<NORMAL_SIZE>{};
class SmallPacket	: public PacketBase<SMALL_SIZE> {};
class LargePacket	: public PacketBase<LARGE_SIZE> {};
