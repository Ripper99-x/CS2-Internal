#include "Aimbot.h"


inline QAngle_t CalculateAngleScalar(const Vector_t& src, const Vector_t& dst)
{
    QAngle_t Angles;
    Vector_t Delta = { dst.x - src.x, dst.y - src.y, dst.z - src.z };
    float hyp = std::sqrt(Delta.x * Delta.x + Delta.y * Delta.y);
    Angles.x = std::atan2(-Delta.z, hyp) * (180.0f / M_PI);
    Angles.y = std::atan2(Delta.y, Delta.x) * (180.0f / M_PI);
    Angles.z = 0.0f;
    return Angles;
}


VOID SilentAim(CUserCmd* pCmd)
{
    if (!MenuConfig::EnableSilentAim || SDK::LocalPawn == nullptr)
        return;

    float ClosestDistance = FLT_MAX;
    PlayerEntity* ClosestEntity = nullptr;

    for (auto& Entity : *CurrentPlayerList)
    {
        if (Entity.Pawn.BoneData.BonePositions.empty())
            continue;

        bool CanAim = Entity.Pawn.isVisible;

        if (!CanAim)
        {
            for (const auto& BonePosition : Entity.Pawn.BoneData.BonePositions)
            {
                if (BonePosition.RayTraceVisible)
                {
                    CanAim = true;
                    break;
                }
            }
        }

        if (!CanAim)
            continue;

        float DistanceToLocal = SDK::LocalPawn->GetOrigin().DistTo(Entity.Pawn.m_vOldOrigin);

        if (DistanceToLocal < ClosestDistance)
        {
            ClosestDistance = DistanceToLocal;
            ClosestEntity = &Entity;
        }
    }

    if (ClosestEntity != nullptr)
    {
        QAngle_t TargetAngles = CalculateAngleScalar(SDK::LocalPawn->GetOrigin() + SDK::LocalPawn->GetViewOffset(), ClosestEntity->Pawn.BoneData.BonePositions[BONEINDEX::Head].Location);
        TargetAngles.Normalize();
        TargetAngles.Clamp();


        QAngle_t PunchAngle = SDK::LocalPawn->GetPunchAngle();
        TargetAngles.x -= PunchAngle.x * 2.0f;
        TargetAngles.y -= PunchAngle.y * 2.0f;


        pCmd->SetSubTickAngle(TargetAngles);
    }
}

VOID Aimbot(CCSGOInput* Input)
{

    if (!MenuConfig::EnableAimbot || SDK::LocalPawn == nullptr)
        return;

    Vector2D_t MousePos = { ImGui::GetMousePos().x,  ImGui::GetMousePos().y };

    float MinDistanceToMouse = FLT_MAX;
    Vector_t ClosestBoneWorldPos;

    auto AimAtBone = [&](const std::vector<BonePosition>& BonePositions, int i)
        {
            if (BonePositions.empty() || i >= BonePositions.size())
                return;

            Vector_t BoneWorldPos = BonePositions[i].Location;
            Vector2D_t BoneScreenPos;

            if (View.WorldToScreen(BoneWorldPos, BoneScreenPos))
            {
                float Distance = MousePos.DistanceTo(BoneScreenPos);

                if (Distance < MinDistanceToMouse && BonePositions[i].RayTraceVisible)
                {
                    MinDistanceToMouse = Distance;
                    ClosestBoneWorldPos = BoneWorldPos;
                }
            }
        };

    if (MenuConfig::TargetClosestVisibleBoneToFov)
        for (auto& Entity : *CurrentPlayerList)
            for (int i = 0; i < Entity.Pawn.BoneData.BonePositions.size(); i++)
                AimAtBone(Entity.Pawn.BoneData.BonePositions, i);

    else
        for (auto& Entity : *CurrentPlayerList)
            AimAtBone(Entity.Pawn.BoneData.BonePositions, MenuConfig::SelectedBoneIndex);

    if (MinDistanceToMouse <= MenuConfig::FOV && GetAsyncKeyState(HotKeyCodes[MenuConfig::SelectedHotkey]) & 0x8000)
    {
        Vector_t PlayerViewPosition = SDK::LocalPawn->GetOrigin() + SDK::LocalPawn->GetViewOffset();
        QAngle_t TargetAngles = CalculateAngleScalar(PlayerViewPosition, ClosestBoneWorldPos);

        if (SDK::LocalPawn->GetShotsFired() > 0)
        {
            QAngle_t PunchAngle = SDK::LocalPawn->GetPunchAngle();
            TargetAngles.x -= PunchAngle.x * 2.0f;
            TargetAngles.y -= PunchAngle.y * 2.0f;
        }

        TargetAngles.Normalize();
        TargetAngles.Clamp();

        QAngle_t CurrentAngles = Input->GetViewAngles();
        float SmoothFactor = 1.0f / MenuConfig::AimSmoothness;
        QAngle_t DeltaAngles(
            (TargetAngles.x - CurrentAngles.x) * SmoothFactor,
            (TargetAngles.y - CurrentAngles.y) * SmoothFactor);

        QAngle_t SmoothAngles = CurrentAngles + DeltaAngles;

        SmoothAngles.Normalize();
        SmoothAngles.Clamp();
        Input->SetViewAngle(SmoothAngles);
    }
}