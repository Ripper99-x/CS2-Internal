#pragma once

#include <windows.h>


#include "detours.h"
#pragma comment( lib, "detours.lib" )

PVOID SetDetour( PVOID& Target, PVOID Handler );
BOOL RemoveDetour( PVOID* ppTarget, PVOID Handler );