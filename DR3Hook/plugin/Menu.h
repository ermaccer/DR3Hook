#pragma once
#include "..\dr3.h"

#include <DirectXMath.h>
using namespace DirectX;

#define DR3HOOK_VERSION "0.1"

enum eMenuSubMenus {
	SM_Settings,
	SM_Creature_List,
	SM_Object_List,
	SM_Total
};


class DR3Menu {
public:
	bool	 m_bIsActive = false;
	bool	 m_bSubmenuActive[SM_Total] = {};
	bool     m_bPressingKey = false;
	bool	 m_bSlowMotion = false;
	bool	 m_bIsFocused = true;
	bool	 m_bFrozeControls = false;

	bool	 m_bCustomCameraPos = false;
	bool	 m_bCustomCameraRot = false;
	bool	 m_bCustomCameraFOV = false;
	bool	 m_bFreeCam = false;
	bool	 m_bMouseControl = false;


	float	 m_fSlowMotionSpeed = 0.5f;
	float	 m_fFreeCamSpeed = 2.25f;
	float    m_fFreeCamRotationSpeed = 0.25f;

	Vector   m_vecCamRotation;

	int* m_pCurrentVarToChange = nullptr;


	char m_szVehicle[256] = {};
	char m_szObject[256] = {};

	DR3Menu();

	void OnActivate();
	void OnToggleSetCameraPos();
	void OnToggleSetCameraRot();
	void OnToggleSetCameraFOV();
	void OnActivateSlowMotion();
	void OnToggleSlowMotion();
	void OnToggleHUD();
	void Draw();

	void Update();
	void UpdateFreeCamera();

	void DrawPlayerTab();
	void DrawCameraTab();
	void DrawSpeedTab();
	void DrawVehicleTab();
	void DrawZombiesTab();
	void DrawLockersTab();
	void DrawWorldTab();
	void DrawMiscTab();

	void DrawSettings();


	void DrawKeyBind(char* name, int* var);
	void KeyBind(int* var, char* bindName, char* name);

};


void Plugin_UpdateThread();

float GetDeltaTime();

extern DR3Menu* TheMenu;