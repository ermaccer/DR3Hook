// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include "gui/eDirectX11Hook.h"
#include "utils/MemoryMgr.h"
#include "utils/addr.h"
#include "utils/Trampoline.h"
#include "plugin/Menu.h"
#include "plugin/Settings.h"
#include "dr3.h"

using namespace Memory::VP;

void OnInitializeHook()
{
	if (SettingsMgr->bEnableConsoleWindow)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
	printf("DR3Hook::OnInitializeHook() | Begin!\n");

	CreateThread(nullptr, 0, DirectXHookThread, 0, 0, nullptr);
	CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Plugin_UpdateThread), nullptr, 0, nullptr);
}

bool ValidateGameVersion()
{
	char* gameName = (char*)_addr(0x1413D3A5C);

	if (strcmp(gameName, "DR3PC") == 0)
		return true;
	else
	{
		MessageBoxA(0, "Invalid game version!\nDR3Hook only supports latest Steam executable,!\n\n"
			"If you still cannot run the plugin and made sure that the game is updated, DR3Hook needs to be updated.", 0, MB_ICONINFORMATION);
		return false;
	}
}




extern "C"
{
	__declspec(dllexport) void InitializeASI()
	{
		if (ValidateGameVersion())
			OnInitializeHook();
	}
}