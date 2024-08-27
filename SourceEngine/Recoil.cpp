#include "Recoil.h"
#include <iostream>
#include <iomanip>

VOID ApplyRecoilControl(CCSGOInput* Input)
{
    if (!MenuConfig::StandloneRCS)
        return;

    QAngle_t CurrentViewAngles = Input->GetViewAngles();

    CUtlVector PunchAngleCache = SDK::LocalPawn->GetCachedAngle();

    if (PunchAngleCache.count == 0 || !PunchAngleCache.data)
        return;

   

    QAngle_t* CurrentPunchAngle = &PunchAngleCache.data[PunchAngleCache.count - 1];

    static QAngle_t PreviousPunchAngle = { 0.0f, 0.0f, 0.0f };

    if (SDK::LocalPawn->GetShotsFired() > 0)
    {
        QAngle_t RecoilCompensation;
        RecoilCompensation.x = (CurrentPunchAngle->x - PreviousPunchAngle.x) * MenuConfig::RCS_X;
        RecoilCompensation.y = (CurrentPunchAngle->y - PreviousPunchAngle.y) * MenuConfig::RCS_Y;
        RecoilCompensation.z = 0.0f;

        QAngle_t TargetAngles = CurrentViewAngles - RecoilCompensation;

        TargetAngles.Normalize();
        TargetAngles.Clamp();

        Input->SetViewAngle(TargetAngles);

        PreviousPunchAngle = *CurrentPunchAngle;
    }

    else
        PreviousPunchAngle = { 0.0f, 0.0f, 0.0f };

   
}