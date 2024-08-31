#include "Hooks.h"
#include "W2S.h"
#include "CRC.h"
extern Vector4D_t Get2DBox(Vector_t WorldPosition, const float Height);
extern VOID Recoil(CCSGOInput* Input);
extern VOID TriggerBot(CCSGOInput* pInput);

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

inline void PlayerEntities(C_BaseEntity* pEntity, SchemaClassInfoData_t* pClassInfo, CCSGOInput* pInput) 
{
	if (FNV1A::Hash(pClassInfo->szName) != FNV1A::HashConst("CCSPlayerController"))
		return;

	CCSPlayerController* Controller = reinterpret_cast<CCSPlayerController*>(pEntity);

	if (Controller == nullptr || Controller->IsLocalPlayerController())
		return;

	C_CSPlayerPawn* Pawn = I::GameResourceService->pGameEntitySystem->Get<C_CSPlayerPawn>(Controller->GetPawnHandle());

	if (Pawn == nullptr)
		return;

	if (Pawn->GetHealth() == 0 && !Controller->IsPawnAlive())
		return;

	if (!Pawn->IsOtherEnemy())
		return;

	CGameSceneNode* GameSceneNode = Pawn->GetGameSceneNode();
	if (GameSceneNode == nullptr)
		return;

	CSkeletonInstance* Skeleton = GameSceneNode->GetSkeletonInstance();
	if (Skeleton == nullptr)
		return;

	C_CSWeaponBase* CSWeaponBase = Pawn->CSWeaponBase();

	if (CSWeaponBase == nullptr)
		return;

	CBoneData Bones;
	PlayerEntity Entity;

	Entity.Controller.m_sSanitizedPlayerName = Controller->GetPlayerName();
	Entity.Pawn.Health = Pawn->GetHealth();
	Entity.Pawn.m_vOldOrigin = Pawn->GetOrigin();
	Entity.Pawn.isVisible = Pawn->EyeVisible(SDK::LocalPawn, Pawn);
	Entity.Pawn.CSWeaponBase = CSWeaponBase;

	Entity.Pawn.Rectangle = Get2DBox(Entity.Pawn.m_vOldOrigin, 75.0f);
	Entity.Pawn.BoneData.BonePositions.clear();
	Entity.Pawn.BoneData.BonePositions.resize(104);

	for (const auto& BoneList : BoneJointList::List) 
	{
		for (const auto& BoneIndex : BoneList) 
		{
			if (BoneIndex >= Entity.Pawn.BoneData.BonePositions.size())
				continue;

			GetBone(Skeleton, Bones, BoneIndex);
			BonePosition Position;
			Position.Location = Bones.Location;
			Position.WorldVisible = View.WorldToScreen(Bones.Location, Position.ScreenPosition);
			Position.RayTraceVisible = Pawn->BoneVisible(SDK::LocalPawn, Pawn, Position.Location);
			Entity.Pawn.BoneData.BonePositions[BoneIndex] = Position;
		}
	}

	Entity.CanHit = Pawn->TriggerHit(SDK::LocalPawn, Pawn, pInput, Entity.Pawn.BoneData);

	NextPlayerList->push_back(Entity);
}


inline VOID WeaponEntities(C_BaseEntity* pEntity, SchemaClassInfoData_t* pClassInfo)
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
				return;

			CGameSceneNode* GameSceneNode = CSWeaponBase->GetGameSceneNode();

			if (GameSceneNode == nullptr)
				return;

			cWeaponEntity->isWeapon = CSWeaponBase->IsWeapon();
			cWeaponEntity->WorldPosition = GameSceneNode->GetOrigin();
			cWeaponEntity->CSWeaponBase = CSWeaponBase;

			NextWeaponList->push_back(*cWeaponEntity);
		}
	}

}


inline VOID ChickenEntities(C_BaseEntity* pEntity, SchemaClassInfoData_t* pClassInfo)
{

	if (FNV1A::Hash(pClassInfo->szName) != FNV1A::HashConst("C_Chicken"))
		return;

	C_Chicken* CChicken = reinterpret_cast<C_Chicken*>(pEntity);

	if (CChicken == nullptr)
		return;

	CGameSceneNode* GameSceneNode = CChicken->GetGameSceneNode();

	if (GameSceneNode == nullptr)
		return;

	CurrentChickenEntity->Chicken.Name = "Chicken";
	CurrentChickenEntity->Chicken.WorldPosition = GameSceneNode->GetOrigin();
	CurrentChickenEntity->Chicken.isVisible = CChicken->ChickenVisible(SDK::LocalPawn, CChicken);
	CurrentChickenEntity->Chicken.Rectangle = Get2DBox(CurrentChickenEntity->Chicken.WorldPosition, 25.0f);

	NextChickenList->push_back(*CurrentChickenEntity);
}


inline VOID InfernoEntities(C_BaseEntity* pEntity, SchemaClassInfoData_t* pClassInfo)
{
	if (FNV1A::Hash(pClassInfo->szName) != FNV1A::HashConst("C_Inferno"))
		return;

	C_Inferno* Molotov = reinterpret_cast<C_Inferno*>(pEntity);

	if (Molotov == nullptr)
		return;

	int EntityID = reinterpret_cast<int>(pEntity);

	if (Molotov->IsBurning())
	{
		auto& Entity = ActiveInfernoEntities[EntityID];
		Entity.Inferno.WorldPosition = Molotov->GetFirePositions();
		Entity.Inferno.Base = Molotov;

		if (Entity.StartTime == std::chrono::steady_clock::time_point())
		{
			Entity.StartTime = std::chrono::steady_clock::now();
			Entity.UniqueInfernoID = NextInfernoID++;
		}
	}
	else
	{
		ActiveInfernoEntities.erase(EntityID);
		return;
	}

}

VOID SkyBox(C_EnvSky* eSky)
{
	if (MenuConfig::LightChanger == false || eSky == nullptr)
		return;

	eSky->m_vTintColorLightingOnly() = Color_t(
		MenuConfig::g_SkyboxColor.x * 255.f,
		MenuConfig::g_SkyboxColor.y * 255.f,
		MenuConfig::g_SkyboxColor.z * 255.f,
		MenuConfig::g_SkyboxColor.w * 255.f
	);

	eSky->m_vTintColor() = Color_t(
		MenuConfig::g_SkyboxColor.x * 255.f,
		MenuConfig::g_SkyboxColor.y * 255.f,
		MenuConfig::g_SkyboxColor.z * 255.f,
		MenuConfig::g_SkyboxColor.w * 255.f
	);

	OriginalUpdateSkybox(eSky);
}

VOID SmokeColor(C_SmokeGrenadeProjectile* Smoke)
{
	if (MenuConfig::EnableSmokeColor && Smoke != nullptr)
		Smoke->m_vSmokeColor() = MenuConfig::SmokeColorPicker;
	
}

bool __fastcall hkCreateMove(CCSGOInput* pInput, int nSlot, bool bActive)
{
	NextPlayerList->clear(), NextWeaponList->clear(), NextChickenList->clear();

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

	SDK::Cmd = pCmd;

	CBaseUserCmdPB* pBaseCmd = pCmd->csgoUserCmd.pBaseCmd;

	if (pBaseCmd == nullptr)
		return bResult;

	for (int nIndex = 1; nIndex <= I::GameResourceService->pGameEntitySystem->GetHighestEntityIndex(); nIndex++)
	{

		C_BaseEntity* pEntity = I::GameResourceService->pGameEntitySystem->Get(nIndex);

		if (pEntity == nullptr)
			continue;

		pEntity->GetSchemaClassInfo(&pClassInfo);

		if (pClassInfo == nullptr)
			continue;

		PlayerEntities(pEntity, pClassInfo, pInput);
		WeaponEntities(pEntity, pClassInfo);
		ChickenEntities(pEntity, pClassInfo);
		InfernoEntities(pEntity, pClassInfo);

		if (FNV1A::Hash(pClassInfo->szName) == FNV1A::HashConst("C_EnvSky"))
			g_pEnvSky = reinterpret_cast<C_EnvSky*>(pEntity);
	
		if (FNV1A::Hash(pClassInfo->szName) == FNV1A::HashConst("C_SmokeGrenadeProjectile"))
			g_pSmokeGrenadeProjectile = reinterpret_cast<C_SmokeGrenadeProjectile*>(pEntity);
		
	}

	std::swap(CurrentPlayerList, NextPlayerList), std::swap(CurrentWeaponList, NextWeaponList), std::swap(CurrentChickenList, NextChickenList);

	SkyBox(g_pEnvSky);
	SmokeColor(g_pSmokeGrenadeProjectile);

	if (SDK::LocalPawn->GetHealth() > 0 && SDK::LocalController->IsPawnAlive())
	{
		SilentAim(pCmd);
		Aimbot(pInput);
		Recoil(pInput);
		TriggerBot(pInput);
	}

	CRC::Save(SDK::Cmd->csgoUserCmd.pBaseCmd);

	if (CRC::CalculateCRC(SDK::Cmd->csgoUserCmd.pBaseCmd) == true)
		CRC::Apply(SDK::Cmd);

	return bResult;
}



void __fastcall hkLightingModulation(__int64 a1, cAggregateSceneObject* SceneObject, __int64 a3)
{

	SceneObject->R = MenuConfig::wModulation.LightingColor.x * MenuConfig::wModulation.LightingIntensity;
	SceneObject->G = MenuConfig::wModulation.LightingColor.y * MenuConfig::wModulation.LightingIntensity;
	SceneObject->B = MenuConfig::wModulation.LightingColor.z * MenuConfig::wModulation.LightingIntensity;

	return oLightingModulation(a1, SceneObject, a3);
}
