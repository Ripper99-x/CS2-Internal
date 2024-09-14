#pragma once
#include "Schema.h"
#include "string"
#include "Utils/FNV1A.h"
#include "../Utils/Interfaces.h"

#define MAX_PATH 260

struct SchemaData_t
{
	FNV1A_t uHashedFieldName;
	std::uint32_t uOffset;
};

inline std::vector<SchemaData_t> SchemaData;


inline bool SetupSchema(const char* szModuleName)
{
	CSchemaSystemTypeScope* pTypeScope = I::SchemaSystem->FindTypeScopeForModule(szModuleName);
	if (pTypeScope == nullptr)
		return false;

	const int nTableSize = pTypeScope->hashClasses.Count();
	UtlTSHashHandle_t* pElements = new UtlTSHashHandle_t[nTableSize + 1U];
	const auto nElements = pTypeScope->hashClasses.GetElements(0, nTableSize, pElements);

	for (int i = 0; i < nElements; i++)
	{
		const UtlTSHashHandle_t hElement = pElements[i];

		if (hElement == 0)
			continue;

		CSchemaClassBinding* pClassBinding = pTypeScope->hashClasses[hElement];
		if (pClassBinding == nullptr)
			continue;

		SchemaClassInfoData_t* pDeclaredClassInfo;
		pTypeScope->FindDeclaredClass(&pDeclaredClassInfo, pClassBinding->szBinaryName);

		if (pDeclaredClassInfo == nullptr)
			continue;

		if (pDeclaredClassInfo->nFieldSize == 0)
			continue;

		for (auto j = 0; j < pDeclaredClassInfo->nFieldSize; j++)
		{
			SchemaClassFieldData_t* pFields = pDeclaredClassInfo->pFields;
			std::string szFieldClassBuffer = std::string(pClassBinding->szBinaryName) + "->" + pFields[j].szName;
			SchemaData.emplace_back(FNV1A::Hash(szFieldClassBuffer.c_str()), pFields[j].nSingleInheritanceOffset);

			std::string szFieldBuffer = std::string(pFields[j].pSchemaType->szName) + " " + pFields[j].szName + " = 0x" + std::to_string(pFields[j].nSingleInheritanceOffset) + "\n";

		}
	}

	delete[] pElements;

	return true;
}

inline std::uint32_t GetOffset(const FNV1A_t uHashedFieldName)
{
	if (const auto it = std::ranges::find_if(SchemaData, [uHashedFieldName](const SchemaData_t& data)
		{ return data.uHashedFieldName == uHashedFieldName; });
		it != SchemaData.end())
		return it->uOffset;

	return 0U;
}


#define SCHEMA_ADD_OFFSET(TYPE, NAME, OFFSET)                                                                 \
	inline std::add_lvalue_reference_t<TYPE> NAME()                                          \
	{                                                                                                         \
		static const std::uint32_t uOffset = OFFSET;                                                          \
		return *reinterpret_cast<std::add_pointer_t<TYPE>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset)); \
	}

#define SCHEMA_ADD_POFFSET(TYPE, NAME, OFFSET)                                                               \
	inline std::add_pointer_t<TYPE> NAME()                                                  \
	{                                                                                                        \
		const static std::uint32_t uOffset = OFFSET;                                                         \
		return reinterpret_cast<std::add_pointer_t<TYPE>>(reinterpret_cast<std::uint8_t*>(this) + (uOffset)); \
	}

#define SCHEMA_ADD_FIELD_OFFSET(TYPE, NAME, FIELD, ADDITIONAL) SCHEMA_ADD_OFFSET(TYPE, NAME, GetOffset(FNV1A::HashConst(FIELD)) + ADDITIONAL)

#define SCHEMA_ADD_FIELD(TYPE, NAME, FIELD) SCHEMA_ADD_FIELD_OFFSET(TYPE, NAME, FIELD, 0U)

#define SCHEMA_ADD_PFIELD_OFFSET(TYPE, NAME, FIELD, ADDITIONAL) SCHEMA_ADD_POFFSET(TYPE, NAME, GetOffset(FNV1A::HashConst(FIELD)) + ADDITIONAL)

#define SCHEMA_ADD_PFIELD(TYPE, NAME, FIELD) SCHEMA_ADD_PFIELD_OFFSET(TYPE, NAME, FIELD, 0U)
