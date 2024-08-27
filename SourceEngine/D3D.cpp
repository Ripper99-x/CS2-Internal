#include "d3d.h"

void InitiateImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	
	CustomFont = io.Fonts->AddFontFromMemoryTTF(Ubuntu, sizeof(Ubuntu), 15.0f);
	WeaponFont = io.Fonts->AddFontFromMemoryTTF(WeaponImages, sizeof(WeaponImages), 20.0f);
	ProjectileFont = io.Fonts->AddFontFromMemoryTTF(WeaponImages, sizeof(WeaponImages), 45.0f);
	TimerFont = io.Fonts->AddFontFromMemoryTTF(Ubuntu, sizeof(Ubuntu), 45.0f);
	DefuseKitFont = io.Fonts->AddFontFromMemoryTTF(WeaponImages, sizeof(WeaponImages), 30.0f);
	
	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX11_Init(pDevice, pContext);

	io.Fonts->TexDesiredWidth = 1024;
	io.Fonts->Build();

	ID3D11ShaderResourceView* fontTexture = nullptr;
	io.Fonts->TexID = (void*)fontTexture;
}



LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

VOID DrawFov()
{
	if (!MenuConfig::DrawFOV)
		return;

	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
	float screenWidth = ImGui::GetIO().DisplaySize.x;
	float screenHeight = ImGui::GetIO().DisplaySize.y;
	ImVec2 center(screenWidth / 2.0f, screenHeight / 2.0f);
	draw_list->AddCircle(center, MenuConfig::FOV, IM_COL32(255, 255, 255, 255), 64, 2.0f);
}

HRESULT WINAPI hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!Initiated)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			Window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &MainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(Window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			InitiateImGui();
			Initiated = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}


	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT) & 1)
		MenuConfig::ShowMenu = !MenuConfig::ShowMenu;

	RenderUI();
	RenderESP();
	DrawFov();
	ImGui::Render();
	pContext->OMSetRenderTargets(1, &MainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pSwapChain, SyncInterval, Flags);
}