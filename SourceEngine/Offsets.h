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

typedef void(__fastcall* LightingModulationHook_t)(__int64 a1, cAggregateSceneObject* SceneObject, __int64 a3);
inline LightingModulationHook_t oLightingModulation = NULL;

typedef __int64(__fastcall* UpdateSkybox_t)(void*);
inline UpdateSkybox_t OriginalUpdateSkybox = NULL;

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
        uintptr_t WorldModulation;
        uintptr_t fnUpdateSky;

    }GameData;

};

inline GameOffsets* Offsets = new GameOffsets;



inline BOOL InitializeOffsets()
{
    PVOID dwCSGOInput = reinterpret_cast<PVOID>(M::FindPattern(CLIENT_DLL, "48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC 48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC 48 89 5C 24 20 57 48 83 ec 20"));
    PVOID dwViewMatrix = reinterpret_cast<PVOID>(M::FindPattern(CLIENT_DLL, "48 8D ? ? ? ? ? 48 C1 E0 06 48 03 C1 C3 CC CC"));
    PVOID GetBoneFunction = reinterpret_cast<PVOID>(M::FindPattern(CLIENT_DLL, "44 8B C0 48 8B D6 48 8B CF E8 ? ? ? ? 48 8B C6"));
    PVOID GetModulationFunction = reinterpret_cast<PVOID>(M::FindPattern(SCENESYSTEM_DLL, "48 89 54 24 ? 53 55 41 57"));
    PVOID UpdateSky = reinterpret_cast<PVOID>(M::FindPattern(CLIENT_DLL, "48 8B C4 48 89 58 18 48 89 70 20 55 57 41 54 41 55"));


    GetBoneFunction = reinterpret_cast<PVOID>(reinterpret_cast<uintptr_t>(GetBoneFunction) + 0x9);
    dwCSGOInput = reinterpret_cast<PVOID>(M::GetAbsoluteAddress(reinterpret_cast<uintptr_t>(dwCSGOInput), 3));
    dwViewMatrix = reinterpret_cast<PVOID>(M::GetAbsoluteAddress(reinterpret_cast<uintptr_t>(dwViewMatrix), 3));
    GetBoneFunction = reinterpret_cast<PVOID>(M::GetAbsoluteAddress(reinterpret_cast<uintptr_t>(GetBoneFunction), 1));

    Offsets->GameData.ReturnAddress = reinterpret_cast<PVOID>(M::FindPattern(CLIENT_DLL, "FF 27"));
    Offsets->GameData.BoneTransform = (CSkeletonInstance__GetBone_t)GetBoneFunction;
    Offsets->GameData.dwViewMatrix = reinterpret_cast<uintptr_t>(dwViewMatrix);
    Offsets->VTable.dwCSGOInput = reinterpret_cast<uintptr_t>(dwCSGOInput);
    Offsets->GameData.WorldModulation = reinterpret_cast<uintptr_t>(GetModulationFunction);
    Offsets->GameData.fnUpdateSky = reinterpret_cast<uintptr_t>(UpdateSky);
    GetBone = Offsets->GameData.BoneTransform;
    OriginalUpdateSkybox = (UpdateSkybox_t)Offsets->GameData.fnUpdateSky;

    return FALSE;
}

