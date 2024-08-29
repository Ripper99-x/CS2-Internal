#include "Entity.h"
#include "Tracing.h"
#include "Utils/Interfaces.h"
#include "Utils/cVector.h"
#include "iEngineClient.h"
#include "GameResourceService.h"


bool C_CSPlayerPawn::BoneVisible(C_CSPlayerPawn* Local, C_CSPlayerPawn* Enemy, Vector_t Location)
{
	Trace_Filter_t Filter = {};
	I::Trace->Init(Filter, Local, 0x1C3003, 4, 7);
	Game_Trace_t Trace = {};
	Ray_t Ray = {};
	Vector_t LocalEye = Local->GetEyePosition();
	Vector_t EnemyEye = Location;
	I::Trace->TraceShape(Ray, &LocalEye, &EnemyEye, Filter, Trace);
	return Trace.HitEntity && Trace.HitEntity->GetRefEHandle().GetEntryIndex() == Enemy->GetRefEHandle().GetEntryIndex() || Trace.Fraction > 0.97f;
}

bool C_CSPlayerPawn::EyeVisible(C_CSPlayerPawn* Local, C_CSPlayerPawn* Enemy)
{
	Trace_Filter_t Filter = {};
	I::Trace->Init(Filter, Local, 0x1C3003, 4, 7);
	Game_Trace_t Trace = {};
	Ray_t Ray = {};
	Vector_t LocalEye = Local->GetEyePosition();
	Vector_t EnemyEye = Enemy->GetEyePosition();
	I::Trace->TraceShape(Ray, &LocalEye, &EnemyEye, Filter, Trace);
	return Trace.HitEntity && Trace.HitEntity->GetRefEHandle().GetEntryIndex() == Enemy->GetRefEHandle().GetEntryIndex() || Trace.Fraction > 0.97f;
}

bool C_Chicken::ChickenVisible(C_CSPlayerPawn* Local, C_Chicken* Enemy)
{
	Trace_Filter_t Filter = {};
	I::Trace->Init(Filter, Local, 0x1C3003, 4, 7);
	Game_Trace_t Trace = {};
	Ray_t Ray = {};
	Vector_t LocalEye = Local->GetEyePosition();
	Vector_t EnemyEye = Enemy->GetEyePosition();
	I::Trace->TraceShape(Ray, &LocalEye, &EnemyEye, Filter, Trace);
	return Trace.HitEntity && Trace.HitEntity->GetRefEHandle().GetEntryIndex() == Enemy->GetRefEHandle().GetEntryIndex() || Trace.Fraction > 0.97f;
}

Vector_t C_CSPlayerPawn::GetEyePosition()
{
	Vector_t EyePosition;
	M::CallVFunc<void, 166>(this, &EyePosition);
	return EyePosition;
}


Vector_t C_Chicken::GetEyePosition()
{
	Vector_t EyePosition;
	M::CallVFunc<void, 166>(this, &EyePosition);
	return EyePosition;
}

CCSPlayerController* CCSPlayerController::GetLocalPlayerController()
{
	const int nIndex = I::Engine->GetLocalPlayer();
	return I::GameResourceService->pGameEntitySystem->Get<CCSPlayerController>(nIndex);
}

C_BaseEntity* C_BaseEntity::GetLocalPlayer()
{
	const int nIndex = I::Engine->GetLocalPlayer();
	return I::GameResourceService->pGameEntitySystem->Get(nIndex);
}

bool C_CSPlayerPawn::IsOtherEnemy()
{
	if (this->GetTeam() == GetLocalPlayer()->GetTeam())
		return false;

	return true;
}


