#pragma once
#include <algorithm>
#include "utils.h"
#pragma pack(push, 1)

/*
	TCP/IP packet
*/
template <size_t BUFFER_SIZE>
class PacketBase
{
public:
	PacketBase() = default;
	~PacketBase() = default;

	void clear();

	// getter setter
	const char* Data()			const { return data_; }
	const unsigned short Size() const { return size_; }
	const bool IsValid()		const { return size_ && !bufferOverflow_; }
	const unsigned short GetReadOffset()  const { return rOffset_; }
	const unsigned short GetWriteOffset() const { return wOffset_; }
	void SetReadOffset(const unsigned short offset)  { return rOffset_ = offset; }
	void SetWriteOffset(const unsigned short offset) { return wOffset_ = offset; }

	// read & write & combine packet data
	template <typename T> 
	PacketBase& operator<<(const T& val);
	template <typename T>
	PacketBase& operator>>(T& val);
	template <size_t BUFFER_SIZE_rhs>
	PacketBase operator+(const PacketBase<BUFFER_SIZE_rhs>& rhs);

	// packet exist
	operator bool();

private:
	template <typename T> 
	void WriteValue(const T& val);

	char data_[BUFFER_SIZE] = { 0 };// packet data
	unsigned short size_	= 0;	// packet size
	unsigned short rOffset_ = 0;	// current packet offset to read
	unsigned short wOffset_ = 0;	// current packet offset to write
	bool bufferOverflow_	= false;// check buffer overflow
};

template<size_t BUFFER_SIZE>
inline void PacketBase<BUFFER_SIZE>::clear()
{
	memset(data_, 0, BUFFER_SIZE);
	size_ = rOffset_ = wOffset_ = 0;
	bufferOverflow_ = false;
}

/*
	write value to data buffer
*/
template<size_t BUFFER_SIZE>
template<typename T>
void PacketBase<BUFFER_SIZE>::WriteValue(const T& val)
{
	// check buffer overflow
	const unsigned short valSize = sizeof(T);
	if (wOffset_ + valSize > BUFFER_SIZE)
	{
		bufferOverflow_ = true;
		return;
	}

	// append "val" to dataBuffer and increase offset
	memcpy(data_ + wOffset_, &val, valSize);
	wOffset_ += valSize;

	// refresh data size
	size_ = std::max(size_, wOffset_);
}

/*
	write value(s) to data buffer
*/
template<size_t BUFFER_SIZE>
template<typename T>
PacketBase<BUFFER_SIZE>& PacketBase<BUFFER_SIZE>::operator<<(const T& val)
{
	if constexpr (is_iterable_v<T> == false)
		WriteValue(val);
	else // vector, list, map, ...
	{
		(*this) << val.size();
		for (const auto& innerVal : val)
			(*this) << innerVal;
	}

	return (*this);
}

/*
	read value from data buffer
*/
template<size_t BUFFER_SIZE>
template<typename T>
PacketBase<BUFFER_SIZE>& PacketBase<BUFFER_SIZE>::operator>>(T& val)
{
	static_assert(is_iterable_v<T> == false, "you must extract (vector, list, map) value one by one");

	// check buffer overflow
	unsigned short valSize = sizeof(T);
	if (rOffset_ + valSize > BUFFER_SIZE)
		valSize = BUFFER_SIZE - rOffset_;

	// read "val" from data buffer and increase offset
	memcpy(&val, data_ + rOffset_, valSize);
	rOffset_ += valSize;

	return (*this);
}

/*
	combine two packet
*/
template<size_t BUFFER_SIZE>
template<size_t BUFFER_SIZE_rhs>
PacketBase<BUFFER_SIZE> PacketBase<BUFFER_SIZE>::operator+(const PacketBase<BUFFER_SIZE_rhs>& rhs)
{
	constexpr unsigned short bufferSize = std::max(BUFFER_SIZE, BUFFER_SIZE_rhs);
	if (bufferSize < this->Size() + rhs.Size())
		bufferSize *= 2;

	PacketBase<bufferSize> newPacket;
	newPacket << this->Data() << rhs.Data();

	return newPacket;
}

template<size_t BUFFER_SIZE>
inline PacketBase<BUFFER_SIZE>::operator bool()
{
	return IsValid();
}

#pragma pack(pop)
