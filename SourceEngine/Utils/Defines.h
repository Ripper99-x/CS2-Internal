#pragma once

#define M_PI 3.14159265358979323846


#define _CS_INTERNAL_CONCATENATE(LEFT, RIGHT) LEFT##RIGHT
#define CS_CONCATENATE(LEFT, RIGHT) _CS_INTERNAL_CONCATENATE(LEFT, RIGHT)
#define MEM_PAD(SIZE)										\
private:												\
	char CS_CONCATENATE(pad_0, __COUNTER__)[SIZE]; \
public:

#define MAX_PATH 260


#define SCHEMASYSTEM_TYPE_SCOPES_OFFSET 0x188
#define SCHEMASYSTEMTYPESCOPE_OFF1 0x3F8
#define SCHEMASYSTEMTYPESCOPE_OFF2 0x8



#define _NUMBER_MAX_BASE 36;

#define CLIENT_DLL (L"client.dll")
#define SCENESYSTEM_DLL (L"scenesystem.dll")
#define ENGINE2_DLL (L"engine2.dll")
#define GAME_RESOURCE_SERVICE_CLIENT ("GameResourceServiceClientV00")
#define SOURCE2_ENGINE_TO_CLIENT ("Source2EngineToClient00")

#define SCHEMASYSTEM_DLL L"schemasystem.dll"
#define SCHEMA_SYSTEM "SchemaSystem_00"
#define TIER0_DLL (L"tier0.dll")
#define DBGHELP_DLL (L"dbghelp.dll")


