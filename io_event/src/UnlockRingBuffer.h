#pragma once
#include <string.h>

#ifndef mb
#define mb()    asm volatile("mfence":::"memory")
#define rmb()   asm volatile("lfence":::"memory")
#define wmb()   asm volatile("sfence":::"memory")
#endif /* mb */

inline unsigned int _nextPowerOfTwo(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

// 环形缓冲区
struct UnlockRingBuffer
{
	unsigned int*				read;		// 读指针
	unsigned int*				write;		// 写指针
	unsigned int				endpos;		// endpos + 1 == capacity
	char*						buffer;		// 数据

	UnlockRingBuffer(unsigned int* _read, unsigned int* _write, char* _buffer, unsigned int _Size)
		: read(_read)
		, write(_write)
		, endpos(0)
		, buffer(NULL)
	{
		unsigned int capacity = _nextPowerOfTwo(_Size);
		endpos = capacity - 1;
		buffer = _buffer;
	}


	void ResetRWPos(void)
	{
		while (*read != 0)
		{
			mb();
			*read = 0;
		}
		while (*write != 0)
		{
			mb();
			*write = 0;
		}
	}


	bool EnsureWriteSize(int iSize)
	{
		int len = endpos + 1 - *write + *read;
		if (len < iSize)
		{
			return false;
		}
		return true;
	}


	// 写数据,不支持多进程
	bool Write(const char* _buffer, unsigned int iSize)
	{
		unsigned int l;
		unsigned int len = endpos + 1 - *write + *read;
		if (len < iSize)
		{
			return false;
		}

		mb();

		unsigned int temp = endpos + 1 - ((*write) & endpos);
		l = (iSize < temp ? iSize : temp);
		memcpy(buffer + ((*write) & endpos), _buffer, l);
		memcpy(buffer, _buffer + l, iSize - l);

		wmb();

		(*write) += iSize;
		return true;
	}

	// 读数据,不支持多进程
	bool Read(char* _buffer, unsigned int iSize)
	{
		unsigned int l;
		unsigned int data_size = *write - *read;
		if (data_size < iSize)
			return false;

		rmb();

		unsigned int temp = endpos + 1 - ((*read) & endpos);
		l = (iSize < temp ? iSize : temp);
		memcpy(_buffer, buffer + ((*read) & endpos), l);
		memcpy(_buffer + l, buffer, iSize - l);

		mb();

		(*read) += iSize;
		return true;
	}
};