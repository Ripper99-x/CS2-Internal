#include "RenderHooks.h"



BOOL PresentHook()
{
    uintptr_t AOBPresent = M::FindPattern(L"gameoverlayrenderer64.dll", "48 8B ?? ?? ?? ?? ?? 48 89 ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 89 ?? ?? ?? 89 74");

    uintptr_t Target = M::GetAbsoluteAddress((AOBPresent), 3);

    if (AOBPresent == 0)
        return FALSE;

    PVOID SwapChain = *reinterpret_cast<PVOID*>(Target + 0x20);


    void** VirtualTableEntry = *reinterpret_cast<void***>(SwapChain);

    oPresent = reinterpret_cast<Present>(VirtualTableEntry[8]);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)oPresent, hkPresent);
    DetourTransactionCommit();


    return TRUE;
}

