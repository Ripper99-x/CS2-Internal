#pragma once
#include "Windows.h"
#include "Entity.h"
#include "cGameEntitySystem.h"
#include "Tracing.h"
#include "CheatMenu.h"
#include "CRC.h"

extern inline QAngle_t CalculateAngleScalar(const Vector_t& src, const Vector_t& dst);

VOID TriggerBot(CCSGOInput* pInput)
{
    if (SDK::Cmd == nullptr || MenuConfig::TriggerBot == false)
        return;

    for (auto& Entity : *CurrentPlayerList)
    {
        if (Entity.CanHit)
        {
            if (SDK::Cmd->csgoUserCmd.pBaseCmd->nClientTick >= SDK::LocalPawn->CSWeaponBase()->m_nNextPrimaryAttackTick())
            {
                SDK::Cmd->csgoUserCmd.pBaseCmd->pInButtonState->nValue |= IN_ATTACK;
                SDK::Cmd->csgoUserCmd.pBaseCmd->pInButtonState->SetBits(BASE_BITS_BUTTONPB);

            }

            else
            {
                SDK::Cmd->csgoUserCmd.pBaseCmd->pInButtonState->nValue &= ~IN_ATTACK;
                SDK::Cmd->csgoUserCmd.pBaseCmd->pInButtonState->SetBits(BASE_BITS_BUTTONPB);
            }
        }
    }
}