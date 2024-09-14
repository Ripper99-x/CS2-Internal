#pragma once
#include "Windows.h"
#include "string"
#include "vector"
#include "cVector.h"
#include "../qAngles.h"
#include "Defines.h"

#ifndef DEG2RAD
#define DEG2RAD(x) ((x) * (M_PI / 180.0f))
#endif


class CUtlBuffer;

namespace M
{
	inline void* GetModuleBaseHandle(const wchar_t* wszModuleName)
	{
		if (wszModuleName == nullptr)
			return GetModuleHandle(nullptr);

		HMODULE hModule = GetModuleHandleW(wszModuleName);


		return hModule;
	}

	template <typename C>
	inline constexpr int StringCompare(const C* tszLeft, const C* tszRight)
	{
		if constexpr (std::is_same_v<C, char>)
		{
			return std::strcmp(tszLeft, tszRight);
		}
		else if constexpr (std::is_same_v<C, wchar_t>)
		{
			return std::wcscmp(tszLeft, tszRight);
		}
	}


	inline void* GetExportAddress(const void* hModuleBase, const char* szProcedureName)
	{
		const auto pBaseAddress = static_cast<const std::uint8_t*>(hModuleBase);

		const auto pIDH = static_cast<const IMAGE_DOS_HEADER*>(hModuleBase);
		if (pIDH->e_magic != IMAGE_DOS_SIGNATURE)
			return nullptr;

		const auto pINH = reinterpret_cast<const IMAGE_NT_HEADERS64*>(pBaseAddress + pIDH->e_lfanew);
		if (pINH->Signature != IMAGE_NT_SIGNATURE)
			return nullptr;

		const IMAGE_OPTIONAL_HEADER64* pIOH = &pINH->OptionalHeader;
		const std::uintptr_t nExportDirectorySize = pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
		const std::uintptr_t uExportDirectoryAddress = pIOH->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

		if (nExportDirectorySize == 0U || uExportDirectoryAddress == 0U)
		{
			return nullptr;
		}

		const auto pIED = reinterpret_cast<const IMAGE_EXPORT_DIRECTORY*>(pBaseAddress + uExportDirectoryAddress);
		const auto pNamesRVA = reinterpret_cast<const std::uint32_t*>(pBaseAddress + pIED->AddressOfNames);
		const auto pNameOrdinalsRVA = reinterpret_cast<const std::uint16_t*>(pBaseAddress + pIED->AddressOfNameOrdinals);
		const auto pFunctionsRVA = reinterpret_cast<const std::uint32_t*>(pBaseAddress + pIED->AddressOfFunctions);

		// Perform binary search to find the export by name
		std::size_t nRight = pIED->NumberOfNames, nLeft = 0U;

		while (nRight != nLeft)
		{
			// Avoid INT_MAX/2 overflow
			const std::size_t uMiddle = nLeft + ((nRight - nLeft) >> 1U);
			const int iResult = StringCompare(szProcedureName, reinterpret_cast<const char*>(pBaseAddress + pNamesRVA[uMiddle]));

			if (iResult == 0)
			{
				const std::uint32_t uFunctionRVA = pFunctionsRVA[pNameOrdinalsRVA[uMiddle]];


				// Check if it's a forwarded export
				if (uFunctionRVA >= uExportDirectoryAddress && uFunctionRVA - uExportDirectoryAddress < nExportDirectorySize)
				{
					// Forwarded exports are not supported
					break;
				}

				return const_cast<std::uint8_t*>(pBaseAddress) + uFunctionRVA;
			}

			if (iResult > 0)
				nLeft = uMiddle + 1;
			else
				nRight = uMiddle;
		}

		return nullptr;
	}

	inline std::uint8_t* ResolveRelativeAddress(std::uint8_t* nAddressBytes, std::uint32_t nRVAOffset, std::uint32_t nRIPOffset)
	{
		std::uint32_t nRVA = *reinterpret_cast<std::uint32_t*>(nAddressBytes + nRVAOffset);
		std::uint64_t nRIP = reinterpret_cast<std::uint64_t>(nAddressBytes) + nRIPOffset;

		return reinterpret_cast<std::uint8_t*>(nRVA + nRIP);
	}

	template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
	static inline T CallVFunc(CBaseClass* thisptr, Args_t... argList)
	{
		using VirtualFn_t = T(__thiscall*)(const void*, decltype(argList)...);
		return (*reinterpret_cast<VirtualFn_t* const*>(reinterpret_cast<std::uintptr_t>(thisptr)))[nIndex](thisptr, argList...);
	}

	inline std::vector<int> PatternToBytes(const char* pattern) {
		std::vector<int> bytes;
		const char* current = pattern;
		while (*current) {
			if (*current == '?') {
				bytes.push_back(-1);  // Wildcard
				current++;
				if (*current == '?') current++;  // Skip second '?'
			}
			else {
				bytes.push_back(static_cast<int>(std::strtoul(current, nullptr, 16)));
				current += 2;
			}
			if (*current == ' ') current++;  // Skip spaces
		}
		return bytes;
	}

	inline uintptr_t FindPattern(const wchar_t* moduleName, const char* pattern) 
	{
		HMODULE hModule = GetModuleHandleW(moduleName);
		if (!hModule) return 0;

		std::uint8_t* moduleBase = reinterpret_cast<std::uint8_t*>(hModule);

		PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(moduleBase + reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase)->e_lfanew);
		std::size_t moduleSize = ntHeaders->OptionalHeader.SizeOfImage;

		std::vector<int> patternBytes = PatternToBytes(pattern);
		std::size_t patternLength = patternBytes.size();

		for (std::size_t i = 0; i <= moduleSize - patternLength; i++) {
			bool found = true;
			for (std::size_t j = 0; j < patternLength; j++) {
				if (patternBytes[j] != -1 && patternBytes[j] != moduleBase[i + j]) {
					found = false;
					break;
				}
			}
			if (found)
				return reinterpret_cast<uintptr_t>(moduleBase + i);

		}

		return 0;
	}


	template <typename T = uintptr_t>
	T GetAbsoluteAddress(T pRelativeAddress, int nPreOffset = 0x0, int nPostOffset = 0x0) {
		std::int32_t relativeOffset = *reinterpret_cast<std::int32_t*>(reinterpret_cast<std::uint8_t*>(pRelativeAddress) + nPreOffset);
		return pRelativeAddress + relativeOffset + sizeof(std::int32_t) + nPreOffset + nPostOffset;
	}

	inline void** CopyVirtualTable(void** Original)
	{
		constexpr int Entries = 8192 / sizeof(void*);

		void** NewVTable = new void* [Entries];

		for (int i = 0; i < Entries; ++i)
			NewVTable[i] = Original[i];


		return NewVTable;
	}

	inline Vector_t AngleToDirection(const QAngle_t& Angles)
	{
		Vector_t Direction;
		float Pitch = DEG2RAD(Angles.x);
		float Yaw = DEG2RAD(Angles.y);

		Direction.x = cos(Pitch) * cos(Yaw);
		Direction.y = cos(Pitch) * sin(Yaw);
		Direction.z = -sin(Pitch);

		return Direction;
	}

	inline void(__fastcall* fnUtlBufferInit)(CUtlBuffer*, int, int, int);
	inline void(__fastcall* fnUtlBufferPutString)(CUtlBuffer*, const char*);
	inline void(__fastcall* fnUtlBufferEnsureCapacity)(CUtlBuffer*, int);
}
