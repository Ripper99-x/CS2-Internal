#pragma once
#include "Schema/Schema.h"
#include "Utils/Defines.h"
#include "Utils/Interfaces.h"
#include "Tracing.h"


inline bool Setup()
{
	bool bSuccess = true;

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

	return bSuccess;
}
