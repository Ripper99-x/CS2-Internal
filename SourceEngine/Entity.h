#pragma once
#include "Utils/Memory.h"
#include "Utils/FNV1A.h"
#include "Schema/SchemaSetup.h"
#include "EntityHandle.h"
#include "Utils/cVector.h"
#include "Bones.h"
#include <memory>

class CEntityIdentity;
class CBaseHandle;
class CSkeletonInstance;
class CGameSceneNode;
class C_BaseEntity;
class C_BaseModelEntity;
class C_BasePlayerPawn;
class C_CSPlayerPawnBase;
class C_CSPlayerPawn;
class CBasePlayerController;
class CCSPlayerController;
class CEntityInstance;
class C_CSWeaponBase;

inline SchemaClassInfoData_t* pClassInfo;

struct PlayerEntity
{
	struct C_CSPlayerController
	{
		std::string m_sSanitizedPlayerName;

	}Controller;

	struct C_CSPlayerPawnBase
	{
		Vector_t m_vOldOrigin{ 0, 0, 0 };
		CBone BoneData;
		C_CSWeaponBase* CSWeaponBase = nullptr;
		Vector4D_t Rectangle;
		bool isVisible = false;
		int Health = 0;
		std::string pClippingWeapon;

	}Pawn;
};

struct WeaponEntity
{

	C_CSWeaponBase* CSWeaponBase = nullptr;
	Vector_t WorldPosition{ 0, 0, 0 };
	std::string Name;
	bool isWeapon = false;

};

struct ChickenEntity
{
	struct CChicken
	{
		Vector_t WorldPosition{ 0, 0, 0 };
		Vector4D_t Rectangle;
		CBone BoneData;
		bool isVisible = false;
		std::string Name;

	}Chicken;
};




inline std::vector<PlayerEntity> ActivePlayersA, ActivePlayersB;
inline std::vector<PlayerEntity>* CurrentPlayerList = &ActivePlayersA, * NextPlayerList = &ActivePlayersB;
inline std::unique_ptr<PlayerEntity> cPlayerEntity = std::make_unique<PlayerEntity>();


inline std::vector<WeaponEntity> WeaponListA, WeaponListB;
inline std::vector<WeaponEntity>* CurrentWeaponList = &WeaponListA, * NextWeaponList = &WeaponListB;
inline std::unique_ptr<WeaponEntity> cWeaponEntity = std::make_unique<WeaponEntity>();

inline std::vector<ChickenEntity> ChickenListA, ChickenListB;
inline std::vector<ChickenEntity>* CurrentChickenList = &ChickenListA, * NextChickenList = &ChickenListB;
inline std::unique_ptr<ChickenEntity> CurrentChickenEntity = std::make_unique<ChickenEntity>();

class CEntityIdentity
{
public:

	SCHEMA_ADD_OFFSET(std::uint32_t, GetIndex, 0x10);
	SCHEMA_ADD_FIELD(const char*, GetDesignerName, "CEntityIdentity->m_designerName");
	SCHEMA_ADD_FIELD(std::uint32_t, GetFlags, "CEntityIdentity->m_flags");

	bool IsValid()
	{
		return GetIndex() != INVALID_EHANDLE_INDEX;
	}

	int GetEntryIndex()
	{
		if (!IsValid())
			return ENT_ENTRY_MASK;

		return GetIndex() & ENT_ENTRY_MASK;
	}

	int GetSerialNumber()
	{
		return GetIndex() >> NUM_SERIAL_NUM_SHIFT_BITS;
	}

	CEntityInstance* pInstance;
};

class CEntityInstance
{
public:

	void GetSchemaClassInfo(SchemaClassInfoData_t** pReturn)
	{
		return M::CallVFunc<void, 38U>(this, pReturn);
	}

	CBaseHandle GetRefEHandle()
	{
		CEntityIdentity* pIdentity = GetIdentity();
		if (pIdentity == nullptr)
			return CBaseHandle();

		return CBaseHandle(pIdentity->GetEntryIndex(), pIdentity->GetSerialNumber() - (pIdentity->GetFlags() & 1));
	}

	SCHEMA_ADD_FIELD(CEntityIdentity*, GetIdentity, "CEntityInstance->m_pEntity");
};


class C_BaseEntity : public CEntityInstance
{
public:

	static C_BaseEntity* GetLocalPlayer();

	bool IsWeapon()
	{
		static SchemaClassInfoData_t* pWeaponBaseClass = nullptr;
		if (pWeaponBaseClass == nullptr)
			I::SchemaSystem->FindTypeScopeForModule("client.dll")->FindDeclaredClass(&pWeaponBaseClass, "C_CSWeaponBase");


		SchemaClassInfoData_t* pClassInfo;
		GetSchemaClassInfo(&pClassInfo);
		if (pClassInfo == nullptr)
			return false;


		return (pClassInfo->InheritsFrom(pWeaponBaseClass));
	}

	SCHEMA_ADD_FIELD(std::uint8_t, GetTeam, "C_BaseEntity->m_iTeamNum");
	SCHEMA_ADD_FIELD(CGameSceneNode*, GetGameSceneNode, "C_BaseEntity->m_pGameSceneNode");
	SCHEMA_ADD_FIELD(std::int32_t, GetHealth, "C_BaseEntity->m_iHealth");

};

class C_BaseModelEntity : public C_BaseEntity
{
public:

	SCHEMA_ADD_FIELD(Vector_t, GetViewOffset, "C_BaseModelEntity->m_vecViewOffset");

};

class C_BasePlayerPawn : public C_BaseModelEntity
{
public:

	SCHEMA_ADD_FIELD(Vector_t, GetOrigin, "C_BasePlayerPawn->m_vOldOrigin");

};

class C_CSPlayerPawnBase : public C_BasePlayerPawn
{
public:

	SCHEMA_ADD_FIELD(C_CSWeaponBase*, CSWeaponBase, "C_CSPlayerPawnBase->m_pClippingWeapon");

};

struct CUtlVector
{
	int count;
	int pad;
	QAngle_t* data;
};

class C_CSPlayerPawn : public C_CSPlayerPawnBase
{
public:

	Vector_t GetEyePosition();
	bool IsOtherEnemy();
	bool BoneVisible(C_CSPlayerPawn* Local, C_CSPlayerPawn* Enemy, Vector_t Location);
	bool EyeVisible(C_CSPlayerPawn* Local, C_CSPlayerPawn* Enemy);
	SCHEMA_ADD_FIELD(QAngle_t, GetPunchAngle, "C_CSPlayerPawn->m_aimPunchAngle");
	SCHEMA_ADD_FIELD(CUtlVector, GetCachedAngle, "C_CSPlayerPawn->m_aimPunchCache");
	SCHEMA_ADD_FIELD(int, GetShotsFired, "C_CSPlayerPawn->m_iShotsFired");



};

class CBasePlayerController : public C_BaseModelEntity
{
public:

	SCHEMA_ADD_FIELD(CBaseHandle, GetPawnHandle, "CBasePlayerController->m_hPawn");
	SCHEMA_ADD_FIELD(bool, IsLocalPlayerController, "CBasePlayerController->m_bIsLocalPlayerController");
	SCHEMA_ADD_FIELD(bool, IsPawnAlive, "CCSPlayerController->m_bPawnIsAlive");

};


class CCSPlayerController : public CBasePlayerController
{
public:

	SCHEMA_ADD_FIELD(const char*, GetPlayerName, "CCSPlayerController->m_sSanitizedPlayerName");
	static CCSPlayerController* GetLocalPlayerController();
	SCHEMA_ADD_FIELD(CBaseHandle, GetPlayerPawnHandle, "CCSPlayerController->m_hPlayerPawn");

};

class CBaseAnimGraph : public C_BaseModelEntity
{
public:

};

class C_BaseFlex : public CBaseAnimGraph
{
public:
};


class C_EconEntity : public C_BaseFlex
{
public:

};


class C_BasePlayerWeapon : public C_EconEntity
{
public:

};

class C_CSWeaponBase : public C_BasePlayerWeapon
{
public:



};

class CGameSceneNode
{
public:
	SCHEMA_ADD_FIELD(CEntityInstance*, GetOwner, "CGameSceneNode->m_pOwner");
	SCHEMA_ADD_FIELD(Vector_t, GetAbsOrigin, "CGameSceneNode->m_vecAbsOrigin");
	SCHEMA_ADD_FIELD(Vector_t, GetRenderOrigin, "CGameSceneNode->m_vRenderOrigin");
	SCHEMA_ADD_FIELD(QAngle_t, GetAngleRotation, "CGameSceneNode->m_angRotation");
	SCHEMA_ADD_FIELD(QAngle_t, GetAbsAngleRotation, "CGameSceneNode->m_angAbsRotation");
	SCHEMA_ADD_FIELD(bool, IsDormant, "CGameSceneNode->m_bDormant");
	SCHEMA_ADD_FIELD(Vector_t, GetOrigin, "CGameSceneNode->m_vecOrigin");

	CSkeletonInstance* GetSkeletonInstance()
	{
		return M::CallVFunc<CSkeletonInstance*, 8U>(this);
	}
};


class CSkeletonInstance : public CGameSceneNode
{
public:

};



class C_Chicken : public C_BaseEntity
{
public:
	Vector_t GetEyePosition();
	bool ChickenVisible(C_CSPlayerPawn* Local, C_Chicken* Enemy);

};
