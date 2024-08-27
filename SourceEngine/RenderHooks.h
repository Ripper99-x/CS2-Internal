#pragma once
#pragma once
#include "dxgi1_4.h"
#include "Utils/Memory.h"
#include <Psapi.h>
#include "D3D.h"


typedef __int64(__fastcall* tPresentCallback)(__int64 a1);
inline tPresentCallback oPresentCallback = NULL;
inline IDXGISwapChain* g_Swapchain = NULL;


