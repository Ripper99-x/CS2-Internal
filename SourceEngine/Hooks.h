#pragma once
#include "Entity.h"
#include "Bones.h"
#include "CCSGOInput.h"
#include "Utils/ReturnAddressr.h"
#include "Offsets.h"
#include "iEngineClient.h"
#include "GameResourceService.h"
#include "Utils/Interfaces.h"
#include "cGameEntitySystem.h"
#include "Aimbot.h"
#include "detour.h"


extern bool __fastcall hkCreateMove(CCSGOInput* pInput, int nSlot, bool bActive);
extern void __fastcall hkLightingModulation(__int64 a1, cAggregateSceneObject* SceneObject, __int64 a3);

namespace Hooks
{
	inline VOID VirtualTable()
	{
		uintptr_t dwCSGOInput = Offsets->VTable.dwCSGOInput;
		uintptr_t CSGOInput = *reinterpret_cast<uintptr_t*>(dwCSGOInput);
		void** CSGOInputVT = reinterpret_cast<void**>(CSGOInput);
		void** NewInputVT = M::CopyVirtualTable(CSGOInputVT);
		void** NewInputClass = M::CopyVirtualTable(reinterpret_cast<void**>(CSGOInput));
		CreateMove = (CreateMove_t)NewInputVT[5];
		NewInputClass[5] = (CreateMove_t)hkCreateMove;

		*reinterpret_cast<void**>(dwCSGOInput) = NewInputClass;
		*reinterpret_cast<void**>(NewInputClass) = NewInputVT;

	}

	inline VOID Detours()
	{
		oLightingModulation = (LightingModulationHook_t)Offsets->GameData.WorldModulation;

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)oLightingModulation, hkLightingModulation);
		DetourTransactionCommit();
	}
}
