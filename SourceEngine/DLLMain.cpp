#include "Windows.h"
#include "Schema/SchemaSetup.h"
#include "Setup.h"
#include "Hooks.h"
#include "RenderHooks.h"



void AllocateConsole()
{
    // Allocate a new console for this process
    if (AllocConsole())
    {
        // Redirect the standard input/output streams to the console
        FILE* file;

        // Redirect STDOUT to the console
        freopen_s(&file, "CONOUT$", "w", stdout);

        // Redirect STDERR to the console
        freopen_s(&file, "CONOUT$", "w", stderr);

        // Redirect STDIN to the console
        freopen_s(&file, "CONIN$", "r", stdin);

        // Optional: Set the console title
        SetConsoleTitle(L"My Application Console");

    }
}

extern BOOL InitializeOffsets();
extern BOOL PresentHook();


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
       // AllocateConsole();
        Setup();
        SetupSchema("client.dll");
        InitializeOffsets();
        PresentHook();
        Hooks::VirtualTable();

        return true;
    }

}