#pragma once
#include <windows.h>
#include <string>
#include <Psapi.h> 
#include "Utils/Memory.h"
#include "Utils/Defines.h"
#include "CCSGOInput.h"
#include "Bones.h"
#include "Entity.h"



typedef bool(__fastcall* CreateMove_t)(CCSGOInput* pInput, int nSlot, bool bActive);
inline CreateMove_t CreateMove = NULL;

using CSkeletonInstance__GetBone_t = void(*)(CSkeletonInstance*, CBoneData&, int);
inline CSkeletonInstance__GetBone_t GetBone = NULL;

typedef __int64(__fastcall* FrameStage_t)(__int64 a1, int a2);
inline FrameStage_t FrameStage = NULL;


class GameOffsets
{
public:
    struct VirtualTable
    {
        uintptr_t dwCSGOInput;
    }VTable;

    struct GameData
    {
        uintptr_t dwViewMatrix;
        CSkeletonInstance__GetBone_t BoneTransform;
        void* ReturnAddress;

    }GameData;

};

inline GameOffsets* Offsets = new GameOffsets;



inline BOOL InitializeOffsets()
{
    PVOID dwCSGOInput = reinterpret_cast<PVOID>(M::FindPattern(CLIENT_DLL, "48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC 48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC 48 89 5C 24 20 57 48 83 ec 20"));
    PVOID dwViewMatrix = reinterpret_cast<PVOID>(M::FindPattern(CLIENT_DLL, "48 8D ? ? ? ? ? 48 C1 E0 06 48 03 C1 C3 CC CC"));
    PVOID GetBoneFunction = reinterpret_cast<PVOID>(M::FindPattern(CLIENT_DLL, "44 8B C0 48 8B D6 48 8B CF E8 ? ? ? ? 48 8B C6"));

    GetBoneFunction = reinterpret_cast<PVOID>(reinterpret_cast<uintptr_t>(GetBoneFunction) + 0x9);
    dwCSGOInput = reinterpret_cast<PVOID>(M::GetAbsoluteAddress(reinterpret_cast<uintptr_t>(dwCSGOInput), 3));
    dwViewMatrix = reinterpret_cast<PVOID>(M::GetAbsoluteAddress(reinterpret_cast<uintptr_t>(dwViewMatrix), 3));
    GetBoneFunction = reinterpret_cast<PVOID>(M::GetAbsoluteAddress(reinterpret_cast<uintptr_t>(GetBoneFunction), 1));

    Offsets->GameData.ReturnAddress = reinterpret_cast<PVOID>(M::FindPattern(CLIENT_DLL, "FF 27"));
    Offsets->GameData.BoneTransform = (CSkeletonInstance__GetBone_t)GetBoneFunction;
    Offsets->GameData.dwViewMatrix = reinterpret_cast<uintptr_t>(dwViewMatrix);
    Offsets->VTable.dwCSGOInput = reinterpret_cast<uintptr_t>(dwCSGOInput);
    GetBone = Offsets->GameData.BoneTransform;

    return FALSE;
}

