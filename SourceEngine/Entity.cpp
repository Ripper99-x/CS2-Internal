#include "Entity.h"
#include "Tracing.h"
#include "Utils/Interfaces.h"
#include "Utils/cVector.h"
#include "iEngineClient.h"
#include "GameResourceService.h"
#include "CheatMenu.h"

extern inline QAngle_t CalculateAngleScalar(const Vector_t& src, const Vector_t& dst);

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


bool C_CSPlayerPawn::TriggerHit(C_CSPlayerPawn* Local, C_CSPlayerPawn* Enemy, CCSGOInput* pInput, const CBone& BoneData)
{
	Trace_Filter_t Filter = {};
	I::Trace->Init(Filter, Local, 0x1C3003, 4, 7);
	Game_Trace_t Trace = {};
	Ray_t Ray = {};
	Vector_t Start = Local->GetEyePosition();

	QAngle_t AdjustedAngles = pInput->GetViewAngles();

	Vector_t Direction = M::AngleToDirection(AdjustedAngles);

	Vector_t TargetPosition = Start + (Direction * 8192.0f);

	I::Trace->TraceShape(Ray, &Start, &TargetPosition, Filter, Trace);

	if (Trace.HitEntity && Trace.HitEntity->GetRefEHandle().GetEntryIndex() == Enemy->GetRefEHandle().GetEntryIndex())
	{
		Vector_t HitPosition = Trace.m_end_pos;

		for (const auto& BonePosition : BoneData.BonePositions)
		{
			if (!BonePosition.RayTraceVisible)
				continue;

			float Distance = (HitPosition - BonePosition.Location).Length();

			if (Distance < MenuConfig::TriggerbotDistanceThreshold)
			{
				QAngle_t ViewAngles = pInput->GetViewAngles();
				QAngle_t TargetAngles = CalculateAngleScalar(HitPosition, BonePosition.Location);

				float AngleDifference = (ViewAngles - TargetAngles).Length();

				if (AngleDifference < MenuConfig::TriggerbotAngleTolerance)
					return true;
		
			}
		}
	}

	return false;
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


