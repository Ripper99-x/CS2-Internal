#include "Hooks.h"
#include "W2S.h"

extern Vector4D_t Get2DBox(Vector_t WorldPosition, const float Height);
extern VOID ApplyRecoilControl(CCSGOInput* Input);

static const std::unordered_map<std::string, size_t> ClassTrimMap =
{
		{"C_Knife", 2},
		{"C_Weapon", 8},
		{"C_DEagle", 2},
		{"C_Flashbang", 2},
		{"C_SmokeGrenade", 2},
		{"C_HEGrenade", 2},
		{"C_IncendiaryGrenade", 2},
		{"C_DecoyGrenade", 2},
		{"C_WeaponSCAR20", 8},
		{"C_C4", 2},
		{"C_AK47", 2},
		{"C_MolotovGrenade", 2},
		{"C_PlantedC4", 9},
		{"CBaseAnimGraph", 0}

};

inline PlayerEntity PlayerEntities(C_BaseEntity* pEntity)
{
	CCSPlayerController* Controller = reinterpret_cast<CCSPlayerController*>(pEntity);

	if (Controller == nullptr || Controller->IsLocalPlayerController())
		return {};

	C_CSPlayerPawn* Pawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(Controller->GetPawnHandle());

	if (Pawn == nullptr)
		return {};

	if (Pawn->GetHealth() == 0 && !Controller->IsPawnAlive())
		return {};

	if (!Pawn->IsOtherEnemy())
		return {};

	CGameSceneNode* GameSceneNode = Pawn->GetGameSceneNode();

	if (GameSceneNode == nullptr)
		return {};

	CSkeletonInstance* Skeleton = GameSceneNode->GetSkeletonInstance();

	if (Skeleton == nullptr)
		return {};


	C_CSWeaponBase* CSWeaponBase = Pawn->CSWeaponBase();

	if (CSWeaponBase == nullptr)
		return {};


	CBoneData Bones;

	cPlayerEntity->Controller.m_sSanitizedPlayerName = Controller->GetPlayerName();
	cPlayerEntity->Pawn.Health = Pawn->GetHealth();
	cPlayerEntity->Pawn.m_vOldOrigin = Pawn->GetOrigin();
	cPlayerEntity->Pawn.isVisible = Pawn->EyeVisible(SDK::LocalPawn, Pawn);
	cPlayerEntity->Pawn.CSWeaponBase = CSWeaponBase;

	cPlayerEntity->Pawn.Rectangle = Get2DBox(cPlayerEntity->Pawn.m_vOldOrigin, 75.0f);
	cPlayerEntity->Pawn.BoneData.BonePositions.clear();
	cPlayerEntity->Pawn.BoneData.BonePositions.resize(30);

	for (const auto& BoneList : BoneJointList::List)
	{
		for (const auto& BoneIndex : BoneList)
		{
			GetBone(Skeleton, Bones, BoneIndex);
			BonePosition Position;
			Position.Location = Bones.Location;
			Position.WorldVisible = View.WorldToScreen(Bones.Location, Position.ScreenPosition);
			Position.RayTraceVisible = Pawn->BoneVisible(SDK::LocalPawn, Pawn, Position.Location);
			cPlayerEntity->Pawn.BoneData.BonePositions[BoneIndex] = Position;
		}
	}

	return *cPlayerEntity;
}


inline WeaponEntity WeaponEntities(C_BaseEntity* pEntity, SchemaClassInfoData_t* pClassInfo)
{
	std::string ClassName = pClassInfo->szName;

	for (const auto& [key, trimLength] : ClassTrimMap)
	{
		if (ClassName.find(key) != std::string::npos)
		{
			ClassName = ClassName.substr(trimLength);
			cWeaponEntity->Name = ClassName;

			C_CSWeaponBase* CSWeaponBase = reinterpret_cast<C_CSWeaponBase*>(pEntity);

			if (CSWeaponBase == nullptr)
				return {};

			CGameSceneNode* GameSceneNode = CSWeaponBase->GetGameSceneNode();

			if (GameSceneNode == nullptr)
				return {};

			cWeaponEntity->isWeapon = CSWeaponBase->IsWeapon();
			cWeaponEntity->WorldPosition = GameSceneNode->GetOrigin();
			cWeaponEntity->CSWeaponBase = CSWeaponBase;

			return *cWeaponEntity;
		}
	}

	return {};
}


inline ChickenEntity ChickenEntities(C_BaseEntity* pEntity)
{
	C_Chicken* CChicken = reinterpret_cast<C_Chicken*>(pEntity);

	if (CChicken == nullptr)
		return {};

	CGameSceneNode* GameSceneNode = CChicken->GetGameSceneNode();

	if (GameSceneNode == nullptr)
		return {};

	CurrentChickenEntity->Chicken.Name = "Chicken";
	CurrentChickenEntity->Chicken.WorldPosition = GameSceneNode->GetOrigin();
	CurrentChickenEntity->Chicken.isVisible = CChicken->ChickenVisible(SDK::LocalPawn, CChicken);
	CurrentChickenEntity->Chicken.Rectangle = Get2DBox(CurrentChickenEntity->Chicken.WorldPosition, 25.0f);

	return *CurrentChickenEntity;
}


bool __fastcall hkCreateMove(CCSGOInput* pInput, int nSlot, bool bActive)
{
	NextPlayerList->clear();
	NextWeaponList->clear();
	NextChickenList->clear();

	const bool bResult = FakeReturnAddress(Offsets->GameData.ReturnAddress, CreateMove, pInput, nSlot, bActive);

	if (!I::Engine->IsConnected() || !I::Engine->IsInGame())
		return bResult;

	SDK::LocalController = CCSPlayerController::GetLocalPlayerController();

	if (SDK::LocalController == nullptr)
		return bResult;

	SDK::LocalPawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(SDK::LocalController->GetPawnHandle());

	if (SDK::LocalPawn == nullptr)
		return bResult;

	CUserCmd* pCmd = pInput->GetUserCmd();

	if (pCmd == nullptr)
		return bResult;

	for (int nIndex = 1; nIndex <= I::GameResourceService->pGameEntitySystem->GetHighestEntityIndex(); nIndex++)
	{

		C_BaseEntity* pEntity = I::GameResourceService->pGameEntitySystem->Get(nIndex);

		if (pEntity == nullptr)
			continue;

		pEntity->GetSchemaClassInfo(&pClassInfo);

		if (pClassInfo == nullptr)
			continue;

		std::string ClassName = pClassInfo->szName;

		if (FNV1A::Hash(pClassInfo->szName) == FNV1A::HashConst("CCSPlayerController"))
		{
			PlayerEntity Entity = PlayerEntities(pEntity);

			if (!Entity.Controller.m_sSanitizedPlayerName.empty())
				NextPlayerList->push_back(Entity);
		}

		else if (FNV1A::Hash(pClassInfo->szName) == FNV1A::HashConst("C_Chicken"))
		{
			ChickenEntity Entity = ChickenEntities(pEntity);

			if (Entity.Chicken.WorldPosition != Vector_t{ 0, 0, 0 })
				NextChickenList->push_back(Entity);
		}

		else if (ClassName.find("C_") == 0 || ClassName.find("CBaseAnimGraph") == 0)
		{
			WeaponEntity Entity = WeaponEntities(pEntity, pClassInfo);

			if (!Entity.Name.empty())
				NextWeaponList->push_back(Entity);
		}

	}

	std::swap(CurrentPlayerList, NextPlayerList);
	std::swap(CurrentWeaponList, NextWeaponList);

	SilentAim(pCmd);
	Aimbot(pInput);

	ApplyRecoilControl(pInput);
	std::swap(CurrentChickenList, NextChickenList);

	SilentAim(pCmd);
	Aimbot(pInput);
	ApplyRecoilControl(pInput);

	return bResult;
}


