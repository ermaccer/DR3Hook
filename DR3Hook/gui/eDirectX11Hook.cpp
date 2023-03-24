#include "eDirectX11Hook.h"
#include "..\plugin\Menu.h"
#include "..\plugin\Settings.h"
#include "font.h"
#include "eNotifManager.h"
#include "..\helper\eKeyboardMan.h"

Present eDirectX11Hook::m_pPresent = {};
ResizeBuffers eDirectX11Hook::m_pResizeBuffers = {};
HWND eDirectX11Hook::ms_hWindow = {};
WNDPROC eDirectX11Hook::ms_pWndProc = {};
ID3D11Device* eDirectX11Hook::pDevice = {};
ID3D11DeviceContext* eDirectX11Hook::pContext = {};
ID3D11RenderTargetView* eDirectX11Hook::mainRenderTargetView = {};
bool eDirectX11Hook::ms_bInit = {};
bool eDirectX11Hook::ms_bFirstDraw = {};
bool eDirectX11Hook::ms_bShouldReloadFonts = {};
ImGuiStyle eDirectX11Hook::ms_localStyleCopy = {};

void eDirectX11Hook::SetImGuiStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowRounding = 6.0f;
	style->ItemSpacing = ImVec2(7, 5.5);
	style->FrameRounding = 2.0f;
	style->FramePadding = ImVec2(6, 4.25);
	ImVec4* colors = style->Colors;

	for (int i = 0; i < ImGuiCol_COUNT; i++)
	{
		ImVec4 col = style->Colors[i];

		if (i == ImGuiCol_Text || i == ImGuiCol_TextDisabled ||
			i == ImGuiCol_WindowBg || i == ImGuiCol_MenuBarBg) continue;

		float avg = (style->Colors[i].x + style->Colors[i].y + style->Colors[i].z) / 3;
		style->Colors[i] = { avg, avg,avg , col.w };
	}

	ms_localStyleCopy = ImGui::GetStyle();


	ReloadImGuiFont();
}

void eDirectX11Hook::InitImGui()
{
	ImGui::CreateContext();
	ImGui::GetIO().ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui_ImplWin32_Init(ms_hWindow);
	ImGui_ImplDX11_Init(pDevice, pContext);
	SetImGuiStyle();

}

void eDirectX11Hook::ReloadImGuiFont()
{

	if (!(SettingsMgr->fMenuScale < 1.0f))
	{
		float fontSize = 16.0f;
		ImGuiStyle * style = &ImGui::GetStyle();
		ImGuiIO io = ImGui::GetIO();

		io.Fonts->Clear();
		io.Fonts->AddFontFromMemoryCompressedTTF(Font_compressed_data, Font_compressed_size, fontSize * SettingsMgr->fMenuScale);
		io.Fonts->Build();

		style->WindowPadding = ms_localStyleCopy.WindowPadding;
		style->WindowRounding = ms_localStyleCopy.WindowRounding;
		style->WindowMinSize = ms_localStyleCopy.WindowMinSize;
		style->ChildRounding = ms_localStyleCopy.ChildRounding;
		style->PopupRounding = ms_localStyleCopy.PopupRounding;
		style->FramePadding = ms_localStyleCopy.FramePadding;
		style->FrameRounding = ms_localStyleCopy.FrameRounding;
		style->ItemSpacing = ms_localStyleCopy.ItemSpacing;
		style->ItemInnerSpacing = ms_localStyleCopy.ItemInnerSpacing;
		style->CellPadding = ms_localStyleCopy.CellPadding;
		style->TouchExtraPadding = ms_localStyleCopy.TouchExtraPadding;
		style->IndentSpacing = ms_localStyleCopy.IndentSpacing;
		style->ColumnsMinSpacing = ms_localStyleCopy.ColumnsMinSpacing;
		style->ScrollbarSize = ms_localStyleCopy.ScrollbarSize;
		style->ScrollbarRounding = ms_localStyleCopy.ScrollbarRounding;
		style->GrabMinSize = ms_localStyleCopy.GrabMinSize;
		style->GrabRounding = ms_localStyleCopy.GrabRounding;
		style->LogSliderDeadzone = ms_localStyleCopy.LogSliderDeadzone;
		style->TabRounding = ms_localStyleCopy.TabRounding;
		style->TabMinWidthForCloseButton = ms_localStyleCopy.TabMinWidthForCloseButton;
		style->DisplayWindowPadding = ms_localStyleCopy.DisplayWindowPadding;
		style->DisplaySafeAreaPadding = ms_localStyleCopy.DisplaySafeAreaPadding;
		style->MouseCursorScale = ms_localStyleCopy.MouseCursorScale;

		style->ScaleAllSizes(SettingsMgr->fMenuScale);
		ImGui_ImplDX11_InvalidateDeviceObjects();

		if (ms_bShouldReloadFonts)
			ms_bShouldReloadFonts = false;


	}
}

HRESULT __stdcall eDirectX11Hook::Present(IDXGISwapChain * pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!ms_bInit)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			ms_hWindow = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			ms_pWndProc = (WNDPROC)SetWindowLongPtr(ms_hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			ms_bInit = true;
		}
		else
			return m_pPresent(pSwapChain, SyncInterval, Flags);
	}
	
	if (ms_bShouldReloadFonts)
		ReloadImGuiFont();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = false;

	static bool ms_bFirstDraw = true;

	if (ms_bFirstDraw)
	{
		Notifications->SetNotificationTime(4500);
		Notifications->PushNotification("DR3Hook %s is running! Press %s to open the menu. Build date: %s\n", DR3HOOK_VERSION,eKeyboardMan::KeyToString(SettingsMgr->iMenuOpenKey), __DATE__);
		ms_bFirstDraw = false;
	}

	Notifications->Draw();

	TheMenu->Draw();

	ImGui::EndFrame();
	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return m_pPresent(pSwapChain, SyncInterval, Flags);
}

LRESULT __stdcall eDirectX11Hook::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KILLFOCUS:
		TheMenu->m_bIsFocused = false;
		break;
	case WM_SETFOCUS:
		TheMenu->m_bIsFocused = true;
		break;
	case WM_KEYDOWN:
		if (wParam == SettingsMgr->iMenuOpenKey)
			TheMenu->OnActivate();
		if (wParam == SettingsMgr->iToggleHUDKey)
			TheMenu->OnToggleHUD();
		if (wParam == SettingsMgr->iToggleSlowMoKey)
			TheMenu->OnToggleSlowMotion();
		break;
	default:
		break;
	}

	if (TheMenu->m_bIsActive)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	 	return true;
	}

	return CallWindowProc(ms_pWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall eDirectX11Hook::ResizeBuffers(IDXGISwapChain * pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	// somehow pdevice handle is missing in mk11 and i2? no idea
	if (pDevice)
	{
		if (mainRenderTargetView)
		{
			pContext->OMSetRenderTargets(0, 0, 0);
			mainRenderTargetView->Release();
		}
	}


	HRESULT result = m_pResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	if (pDevice)
	{
		ID3D11Texture2D* pBackBuffer;
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
		pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		pBackBuffer->Release();

		D3D11_VIEWPORT viewport;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)Width;
		viewport.Height = (float)Height;
		
		
		
		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		pContext->RSSetViewports(1, &viewport);

	}

	return  result;
}

DWORD __stdcall DirectXHookThread(LPVOID lpReserved)
{
	if (SettingsMgr->iHookingTimer > 0)
		Sleep(SettingsMgr->iHookingTimer);
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&eDirectX11Hook::m_pPresent, eDirectX11Hook::Present);
			kiero::bind(13, (void**)&eDirectX11Hook::m_pResizeBuffers, eDirectX11Hook::ResizeBuffers);
			init_hook = true;
		}
	} while (!init_hook);

	return TRUE;
}

