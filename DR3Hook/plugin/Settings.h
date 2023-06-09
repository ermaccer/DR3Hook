#pragma once
#include <Windows.h>


class eSettingsManager {
public:
	eSettingsManager();

	bool bEnableConsoleWindow;

	int iMenuOpenKey = VK_OEM_3;
	int iHookingTimer;
	int iToggleSlowMoKey;
	int iToggleHUDKey;


	// free camera keys
	int iFreeCameraKeyForward;
	int iFreeCameraKeyBack;
	int iFreeCameraKeyUp;
	int iFreeCameraKeyDown;
	int iFreeCameraKeyLeft;
	int iFreeCameraKeyRight;

	int iFreeCameraKeyYawPlus;
	int iFreeCameraKeyYawMinus;
	int iFreeCameraKeyPitchPlus;
	int iFreeCameraKeyPitchMinus;
	int iFreeCameraKeyRollPlus;
	int iFreeCameraKeyRollMinus;

	int iFreeCameraKeyFOVPlus;
	int iFreeCameraKeyFOVMinus;

	float fMenuScale;

	void SaveSettings();
	void ResetKeys();

};

extern eSettingsManager* SettingsMgr;