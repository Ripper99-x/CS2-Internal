#include "Windows.h"
#include "Schema/SchemaSetup.h"
#include "Setup.h"
#include "Hooks.h"
#include "RenderHooks.h"



void AllocateConsole()
{
    if (AllocConsole())
    {
        FILE* file;

        freopen_s(&file, "CONOUT$", "w", stdout);

        freopen_s(&file, "CONOUT$", "w", stderr);

        freopen_s(&file, "CONIN$", "r", stdin);

        SetConsoleTitle(L"My Application Console");

    }
}

extern BOOL InitializeOffsets();
extern BOOL PresentHook();


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        AllocateConsole();
        Setup();
        SetupSchema("client.dll");
        InitializeOffsets();
        PresentHook();
        Hooks::VirtualTable();
        Hooks::Detours();

        return true;
    }

}