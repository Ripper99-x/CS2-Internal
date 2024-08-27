#pragma once
#include <d3d11.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "WeaponFont.h"
#include "TextFont.h"
#include "CheatMenu.h"
#include "ESP.h"
#include "cGameEntitySystem.h"
#include "detour.h"




LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID InitiateImGui();


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
inline Present oPresent;
HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
inline LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
inline WNDPROC oWndProc;
inline HWND Window = 0;
inline ID3D11Device* pDevice = 0;
inline ID3D11DeviceContext* pContext = 0;
inline ID3D11RenderTargetView* MainRenderTargetView;
inline bool  g_WantUpdateHasGamepad = true;
inline bool  Initiated = false;
inline ImFont* CustomFont = nullptr;
inline ImFont* WeaponFont = nullptr;
inline ImFont* ProjectileFont = nullptr;
inline ImFont* DefuseKitFont = nullptr;
inline ImFont* TimerFont = nullptr;