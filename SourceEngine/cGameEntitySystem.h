#pragma once
#include "EntityHandle.h"
#include "Utils/Memory.h"
#include "Utils/Defines.h"
#include "Entity.h"
#include "CCSGOInput.h"

#define MAX_ENTITIES_IN_LIST 512
#define MAX_ENTITY_LISTS 64
#define MAX_TOTAL_ENTITIES MAX_ENTITIES_IN_LIST* MAX_ENTITY_LISTS

class C_BaseEntity;

class CGameEntitySystem
{
public:
	template <typename T = C_BaseEntity>
	T* Get(int nIndex)
	{
		return reinterpret_cast<T*>(this->GetEntityByIndex(nIndex));
	}

	template <typename T = C_BaseEntity>
	T* Get(const CBaseHandle hHandle)
	{
		if (!hHandle.IsValid())
			return nullptr;

		return reinterpret_cast<T*>(this->GetEntityByIndex(hHandle.GetEntryIndex()));
	}

	int GetHighestEntityIndex()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + 0x1510);
	}

private:
	void* GetEntityByIndex(int nIndex)
	{
		using fnGetBaseEntity = void* (__thiscall*)(void*, int);
		static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(M::FindPattern(CLIENT_DLL, "81 FA ? ? ? ? 77 ? 8B C2 C1 F8 ? 83 F8 ? 77 ? 48 98 48 8B 4C C1 ? 48 85 C9 74 ? 8B C2 25 ? ? ? ? 48 6B C0 ? 48 03 C8 74 ? 8B 41 ? 25 ? ? ? ? 3B C2 75 ? 48 8B 01"));
		return GetBaseEntity(this, nIndex);
	}
};




namespace SDK
{
	inline CCSPlayerController* LocalController = nullptr;
	inline C_CSPlayerPawn* LocalPawn = nullptr;
	inline CUserCmd* Cmd = nullptr;
}


