#pragma once
#include "Utils/Memory.h"

class CUtlBuffer
{
public:
	MEM_PAD(0x80);

	CUtlBuffer(int a1, int nSize, int a3)
	{
		M::fnUtlBufferInit != nullptr;

		M::fnUtlBufferInit(this, a1, nSize, a3);
	}

	void PutString(const char* szString)
	{
		M::fnUtlBufferPutString != nullptr;

		M::fnUtlBufferPutString(this, szString);
	}

	void EnsureCapacity(int nSize)
	{
		M::fnUtlBufferEnsureCapacity != nullptr;

		M::fnUtlBufferEnsureCapacity(this, nSize);
	}
};
