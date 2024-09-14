#pragma once
#include "Schema/Schema.h"
#include "Utils/Defines.h"
#include "Utils/Interfaces.h"
#include "Tracing.h"


inline bool Setup()
{
	bool bSuccess = true;

	const void* hDbgHelp = M::GetModuleBaseHandle(DBGHELP_DLL);
	const void* hTier0 = M::GetModuleBaseHandle(TIER0_DLL);

	if (hDbgHelp == nullptr || hTier0 == nullptr)
		return false;


	const auto pTier0Handle = M::GetModuleBaseHandle(TIER0_DLL);
	if (pTier0Handle == nullptr)
		return false;

	const auto pSchemaSystemRegisterList = I::GetRegisterList(SCHEMASYSTEM_DLL);
	if (pSchemaSystemRegisterList == nullptr)
		return false;

	I::SchemaSystem = I::Capture<ISchemaSystem>(pSchemaSystemRegisterList, SCHEMA_SYSTEM);
	bSuccess &= (I::SchemaSystem != nullptr);

	const auto pEngineRegisterList = I::GetRegisterList(ENGINE2_DLL);
	if (pEngineRegisterList == nullptr)
		return false;

	I::GameResourceService = I::Capture<IGameResourceService>(pEngineRegisterList, GAME_RESOURCE_SERVICE_CLIENT);
	bSuccess &= (I::GameResourceService != nullptr);

	I::Engine = I::Capture<IEngineClient>(pEngineRegisterList, SOURCE2_ENGINE_TO_CLIENT);
	bSuccess &= (I::Engine != nullptr);

	I::Trace = *reinterpret_cast<Tracing**>(M::GetAbsoluteAddress(M::FindPattern(CLIENT_DLL, "4C 8B 3D ? ? ? ? 24 C9 0C 49 66 0F 7F 45 ?"), 0x3));

	bSuccess &= (I::Trace != nullptr);

	I::MemAlloc = *reinterpret_cast<IMemAlloc**>(M::GetExportAddress(pTier0Handle, "g_pMemAlloc"));
	bSuccess &= (I::MemAlloc != nullptr);

	M::fnUtlBufferInit = reinterpret_cast<decltype(M::fnUtlBufferInit)>(M::GetExportAddress(hTier0, "??0CUtlBuffer@@QEAA@HHH@Z"));
	bSuccess &= (M::fnUtlBufferInit != nullptr);

	M::fnUtlBufferPutString = reinterpret_cast<decltype(M::fnUtlBufferPutString)>(M::GetExportAddress(hTier0, "?PutString@CUtlBuffer@@QEAAXPEBD@Z"));
	bSuccess &= (M::fnUtlBufferPutString != nullptr);

	M::fnUtlBufferEnsureCapacity = reinterpret_cast<decltype(M::fnUtlBufferEnsureCapacity)>(M::GetExportAddress(hTier0, "?EnsureCapacity@CUtlBuffer@@QEAAXH@Z"));
	bSuccess &= (M::fnUtlBufferEnsureCapacity != nullptr);

	return bSuccess;
}
