#pragma once
#include "Memory.h"

using InstantiateInterfaceFn_t = void* (*)();

class ISchemaSystem;
class IGameResourceService;
class IEngineClient;
class IEngineCVar;
class Tracing;


class CInterfaceRegister
{
public:
	InstantiateInterfaceFn_t fnCreate;
	const char* szName;
	CInterfaceRegister* pNext;
};

namespace I
{
	static const CInterfaceRegister* GetRegisterList(const wchar_t* wszModuleName)
	{
		void* hModule = M::GetModuleBaseHandle(wszModuleName);
		if (hModule == nullptr)
			return nullptr;

		std::uint8_t* pCreateInterface = reinterpret_cast<std::uint8_t*>(M::GetExportAddress(hModule, "CreateInterface"));

		if (pCreateInterface == nullptr)
		{
			return nullptr;
		}

		return *reinterpret_cast<CInterfaceRegister**>(M::ResolveRelativeAddress(pCreateInterface, 0x3, 0x7));
	}



	template <typename T = void*>
	T* Capture(const CInterfaceRegister* pModuleRegister, const char* szInterfaceName)
	{
		std::size_t nInterfaceNameLength = std::strlen(szInterfaceName);

		for (const CInterfaceRegister* pRegister = pModuleRegister; pRegister != nullptr; pRegister = pRegister->pNext)
		{
			std::size_t nRegisterNameLength = std::strlen(pRegister->szName);

			if (std::strncmp(szInterfaceName, pRegister->szName, nInterfaceNameLength) == 0 &&
				(nRegisterNameLength == nInterfaceNameLength ||
					std::strtol(pRegister->szName + nInterfaceNameLength, nullptr, 10) > 0))
			{
				void* pInterface = pRegister->fnCreate();
				return static_cast<T*>(pInterface);
			}
		}

		return nullptr;
	}



	inline ISchemaSystem* SchemaSystem = nullptr;
	inline Tracing* Trace = nullptr;
	inline IGameResourceService* GameResourceService = nullptr;
	inline IEngineClient* Engine = nullptr;
	inline IEngineCVar* Cvar = nullptr;

}

