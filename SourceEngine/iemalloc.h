#pragma once
// used: mem::CallVFunc
#include "Utils/Memory.h"
#pragma warning(push)
#pragma warning(disable : 4191)

class IMemAlloc
{
public:
	void* Alloc(std::size_t nSize)
	{
		return M::CallVFunc<void*, 1>(this, nSize);
	}

	void* ReAlloc(const void* pMemory, std::size_t nSize)
	{
		return M::CallVFunc<void*, 2>(this, pMemory, nSize);
	}

	void Free(const void* pMemory)
	{
		return M::CallVFunc<void, 3>(this, pMemory);
	}

	std::size_t GetSize(const void* pMemory)
	{
		return M::CallVFunc<std::size_t, 21>(this, pMemory);
	}
};

#pragma warning(pop)