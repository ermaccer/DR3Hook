#include <iostream>

#include "menu.h"
#include "..\gui\imgui\imgui.h"
#include "..\utils\addr.h"
#include "..\utils\MemoryMgr.h"
#include "..\gui\eDirectX11Hook.h"
#include "..\helper\eKeyboardMan.h"
#include "..\gui\eNotifManager.h"
#include "settings.h"


using namespace Memory::VP;

DR3Menu* TheMenu = new DR3Menu();

static const char* szVehicles[] = {
		"MotorcycleObject",
		"SportBikeObject",
		"SportsCar2Object",
		"HearseObject",
		"LowRiderObject",
		"LowRiderFuelDrumObject",
		"SUVObject",
		"SedanObject",
		"MuscleCarObject",
		"VanObject",
		"Van_ContrabandObject",
		"Van_PartyObject",
		"Van_FireworksObject",
		"Van_CollinsObject",
		"AmbulanceObject",
		"ForkliftObject",
		"StreetCleanerObject",
		"SteamrollerObject",
		"BulldozerObject",
		"Driving_schoolObject",
		"TaxiObject",
		"PoliceObject",
		"dlc_musclecar_object",
		"dlc_apc_object",
		"dlc_apc_object_no_fire",
		"dlc_rollerhog_object",
		"dlc_foodtruck_object",
		"dlc_foodtruck_object_no_fire",
		"dlc_chopper_object",
		"dlc_bikeslicer_object",
		"dlc_rollerhog_mtndew_object",
		"dlc_zdc_truck_object",
		"dlc_zdc_truck_no_fire_object",
		"cv_rollerhog_object",
		"tricycleObject",
		"cv_forkwork_object",
		"cv_party_slapper_object",
		"cv_shockdozer2_object",
		"JunkBikeObject",
		"JunkSmallCarObject",
		"JunkLargeCarObject",
		"cv_mobile_locker_object",
		"cv_turret_rig_object",
};


DR3Menu::DR3Menu()
{
	sprintf(m_szVehicle, szVehicles[0]);
}

void DR3Menu::OnActivate()
{
	TheMenu->m_bIsActive ^= 1;

	if (TheMenu->m_bIsActive && !TheMenu->m_bFrozeControls)
	{
		SetControllerMode(CM_FE, 0);
		TheMenu->m_bFrozeControls = true;
	}
	else if (!TheMenu->m_bIsActive && TheMenu->m_bFrozeControls)
	{
		SetControllerMode(CM_Play, 0);
		TheMenu->m_bFrozeControls = false;
	}
}

void DR3Menu::OnToggleSetCameraPos()
{
	if (m_bCustomCameraPos)
	{
		Nop(_addr(0x1408A6A16), 3);
		Nop(_addr(0x1408A6A20), 3);
		Nop(_addr(0x1408A6A2D), 3);

	}
	else
	{
		Patch<char>(_addr(0x1408A6A16),		0x89);
		Patch<char>(_addr(0x1408A6A16) + 1, 0x4F);
		Patch<char>(_addr(0x1408A6A16) + 2, 0x28);

		Patch<char>(_addr(0x1408A6A20),		0x89);
		Patch<char>(_addr(0x1408A6A20) + 1, 0x4F);
		Patch<char>(_addr(0x1408A6A20) + 2, 0x2C);

		Patch<char>(_addr(0x1408A6A2D),		0x89);
		Patch<char>(_addr(0x1408A6A2D) + 1, 0x4F);
		Patch<char>(_addr(0x1408A6A2D) + 2, 0x30);
	}
}

void DR3Menu::OnToggleSetCameraRot()
{
	m_vecCamRotation = {};
	if (m_bCustomCameraRot)
	{
		// fwd
		Nop(_addr(0x1408A6A34), 3);
		Nop(_addr(0x1408A6A3F), 3);
		Nop(_addr(0x1408A6A46), 3);

		// up
		Nop(_addr(0x1408A6A4D), 3);
		Nop(_addr(0x1408A6A54), 3);
		Nop(_addr(0x1408A6A5B), 3);
	}
	else
	{
		// fwd
		Patch<char>(_addr(0x1408A6A34), 0x89);
		Patch<char>(_addr(0x1408A6A34) + 1, 0x4F);
		Patch<char>(_addr(0x1408A6A34) + 2, 0x34);

		Patch<char>(_addr(0x1408A6A3F), 0x89);
		Patch<char>(_addr(0x1408A6A3F) + 1, 0x47);
		Patch<char>(_addr(0x1408A6A3F) + 2, 0x38);

		Patch<char>(_addr(0x1408A6A46), 0x89);
		Patch<char>(_addr(0x1408A6A46) + 1, 0x47);
		Patch<char>(_addr(0x1408A6A46) + 2, 0x3C);

		// up
		Patch<char>(_addr(0x1408A6A4D), 0x89);
		Patch<char>(_addr(0x1408A6A4D) + 1, 0x47);
		Patch<char>(_addr(0x1408A6A4D) + 2, 0x40);

		Patch<char>(_addr(0x1408A6A54), 0x89);
		Patch<char>(_addr(0x1408A6A54) + 1, 0x47);
		Patch<char>(_addr(0x1408A6A54) + 2, 0x44);

		Patch<char>(_addr(0x1408A6A5B), 0x89);
		Patch<char>(_addr(0x1408A6A5B) + 1, 0x47);
		Patch<char>(_addr(0x1408A6A5B) + 2, 0x48);
	}
}

void DR3Menu::OnToggleSetCameraFOV()
{
	if (m_bCustomCameraFOV)
	{
		Nop(_addr(0x1408A6A62), 3);
	}
	else
	{
		Patch<char>(_addr(0x1408A6A62), 0x89);
		Patch<char>(_addr(0x1408A6A62) + 1, 0x47);
		Patch<char>(_addr(0x1408A6A62) + 2, 0x4C);
	}
}

void DR3Menu::OnActivateSlowMotion()
{
	if (!GetGame())
		return;

	if (m_bSlowMotion)
		GetGame()->SetSpeed(m_fSlowMotionSpeed);
	else
		GetGame()->SetSpeed(1.0f);
}

void DR3Menu::OnToggleSlowMotion()
{
	TheMenu->m_bSlowMotion ^= 1;
	OnActivateSlowMotion();
}

void DR3Menu::OnToggleHUD()
{
	bool& drawfe = *(bool*)(_addr(0x1416642D1));
	drawfe ^= 1;
}

void DR3Menu::Draw()
{
	if (!m_bIsActive)
		return;

	ImGui::GetIO().MouseDrawCursor = true;

	ImGui::Begin("DR3Hook by ermaccer", &m_bIsActive, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Settings"))
			{
				m_bSubmenuActive[SM_Settings] = true;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::BeginMenu("About"))
				{
					ImGui::MenuItem("Version: " DR3HOOK_VERSION);
					ImGui::MenuItem("Date: " __DATE__);
					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::BeginTabBar("##tabs"))
		{
			if (ImGui::BeginTabItem("Camera"))
			{
				DrawCameraTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Speed"))
			{
				DrawSpeedTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Vehicles"))
			{
				DrawVehicleTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Zombies"))
			{
				DrawZombiesTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Lockers"))
			{
				DrawLockersTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("World"))
			{
				DrawWorldTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc."))
			{
				DrawMiscTab();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	ImGui::End();

	if (m_bSubmenuActive[SM_Settings])
		DrawSettings();
}

void DR3Menu::Update()
{
	if (GetGame() == nullptr)
		return;

	if (m_bSlowMotion)
		GetGame()->SetSpeed(m_fSlowMotionSpeed);

	UpdateFreeCamera();
}

void DR3Menu::UpdateFreeCamera()
{
	cViewport* view = GetGame()->GetViewport(VIEWPORT_MAIN);

	if (!view)
		return;

	Vector fwd = view->mCameraView;
	Vector up = view->mCameraUp;
	Vector right = CrossProduct(fwd, Vector(0, 1, 0));

	static XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	static XMVECTOR defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	if (m_bFreeCam)
	{
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyForward))
			view->mCameraPos += fwd * TheMenu->m_fFreeCamSpeed * GetDeltaTime() * 1;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyBack))
			view->mCameraPos += fwd * TheMenu->m_fFreeCamSpeed * GetDeltaTime() * -1;

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyLeft))
			view->mCameraPos += right * TheMenu->m_fFreeCamSpeed * GetDeltaTime() * -1;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRight))
			view->mCameraPos += right * TheMenu->m_fFreeCamSpeed * GetDeltaTime() * 1;

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyUp))
			view->mCameraPos.Y += TheMenu->m_fFreeCamSpeed * GetDeltaTime() * 1;
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyDown))
			view->mCameraPos.Y += TheMenu->m_fFreeCamSpeed * GetDeltaTime() * -1;


		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyPitchPlus))
			m_vecCamRotation.X += TheMenu->m_fFreeCamRotationSpeed * GetDeltaTime();
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyPitchMinus))
			m_vecCamRotation.X -= TheMenu->m_fFreeCamRotationSpeed * GetDeltaTime();

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYawPlus))
			m_vecCamRotation.Y += TheMenu->m_fFreeCamRotationSpeed * GetDeltaTime();
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyYawMinus))
			m_vecCamRotation.Y -= TheMenu->m_fFreeCamRotationSpeed * GetDeltaTime();

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRollPlus))
			m_vecCamRotation.Z += TheMenu->m_fFreeCamRotationSpeed * GetDeltaTime();
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyRollMinus))
			m_vecCamRotation.Z -= TheMenu->m_fFreeCamRotationSpeed * GetDeltaTime();

		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVMinus))
			view->mFOV -= 1.5f * GetDeltaTime();
		if (GetAsyncKeyState(SettingsMgr->iFreeCameraKeyFOVPlus))
			view->mFOV += 1.5f * GetDeltaTime();
	}


	if (m_bCustomCameraRot)
	{
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(m_vecCamRotation.X, m_vecCamRotation.Y, m_vecCamRotation.Z);
		XMVECTOR camLookAt = XMVector3TransformCoord(defaultForward, rotMat);
		XMVECTOR camUp = XMVector3TransformCoord(defaultUp, rotMat);
		XMVector3Normalize(camLookAt);
		XMVector3Normalize(camUp);

		XMFLOAT3 cL, cU;
		XMStoreFloat3(&cL, camLookAt);
		XMStoreFloat3(&cU, camUp);
		view->mCameraView = { cL.x, cL.y, cL.z };
		view->mCameraUp = { cU.x, cU.y, cU.z };
	}

}

void DR3Menu::DrawPlayerTab()
{
}

void DR3Menu::DrawCameraTab()
{
	if (GetGame() == nullptr)
		return;

	cViewport* view = GetGame()->GetViewport(VIEWPORT_MAIN);

	if (!view)
		return;

	if (ImGui::Checkbox("Set Camera Position", &m_bCustomCameraPos))
		OnToggleSetCameraPos();

	ImGui::InputFloat3("X | Y | Z", &view->mCameraPos.X);

	if (ImGui::Checkbox("Set Camera Rotation", &m_bCustomCameraRot))
		OnToggleSetCameraRot();
	ImGui::InputFloat3("Pitch | Yaw | Roll", &m_vecCamRotation.X);
	if (ImGui::Checkbox("Set FOV", &m_bCustomCameraFOV))
		OnToggleSetCameraFOV();

	ImGui::InputFloat("FOV", &view->mFOV);

	ImGui::Separator();
	ImGui::Checkbox("Free Camera", &m_bFreeCam);
	if (m_bFreeCam)
	{
		if (!m_bCustomCameraPos || !m_bCustomCameraRot || !m_bCustomCameraFOV)
			ImGui::TextColored(ImVec4(1.f, 0.3f, 0.3f, 1.f), "Check rest of the Set Camera options!");

		{
			ImGui::InputFloat("Free Camera Speed", &m_fFreeCamSpeed);
			ImGui::InputFloat("Free Camera Rotation Speed", &m_fFreeCamRotationSpeed);
		}
	}
	ImGui::Separator();
#ifdef _DEBUG
	ImGui::Text("%p", Game->GetViewport(VIEWPORT_MAIN));
#endif // _DEBUG


}

void DR3Menu::DrawSpeedTab()
{
	if (GetGame() == nullptr)
		return;
	ImGui::Text("Gamespeed Control");
	ImGui::InputFloat("", &m_fSlowMotionSpeed, 0.1f);

	if (ImGui::Checkbox("Enable", &m_bSlowMotion))
		OnActivateSlowMotion();
	
}

void DR3Menu::DrawVehicleTab()
{
	ImGui::TextWrapped("Vehicle");
	ImGui::PushItemWidth(-FLT_MIN);
	if (ImGui::BeginCombo("##vehlist", m_szVehicle))
	{
		for (int n = 0; n < IM_ARRAYSIZE(szVehicles); n++)
		{
			bool is_selected = (m_szVehicle == szVehicles[n]);
			if (ImGui::Selectable(szVehicles[n], is_selected))
				sprintf(m_szVehicle, szVehicles[n]);
			if (is_selected)
				ImGui::SetItemDefaultFocus();

		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
	if (ImGui::Button("Create", { -FLT_MIN, 0 }))
	{
		CreateObject(m_szVehicle);
	}
}

void DR3Menu::DrawZombiesTab()
{
	bool& zombie_ai = *(bool*)_addr(0x141713364);
	ImGui::Checkbox("Disable Zombie AI", &zombie_ai);
	ImGui::Separator();
	bool& zombie_scale = *(bool*)(_addr(0x141713469));
	ImGui::Checkbox("Change Zombie Scale", &zombie_scale);
	if (zombie_scale)
	{
		float& zombie_scale_value = *(float*)(_addr(0x141637524));
		ImGui::SliderFloat("Zombie Scale", &zombie_scale_value, 0.0f, 10.0f);
	}
	ImGui::Separator();

}

void DR3Menu::DrawLockersTab()
{
	if (ImGui::Button("Open Weapons Locker", {-FLT_MIN , 0}))
		((void(__fastcall*)())_addr(0x140651A60))();
	if (ImGui::Button("Open Clothes Locker", { -FLT_MIN , 0 }))
		((void(__fastcall*)())_addr(0x140651B40))();
	if (ImGui::Button("Open Survivor Menu", { -FLT_MIN , 0 }))
		((void(__fastcall*)())_addr(0x140651D00))();
}

void DR3Menu::DrawWorldTab()
{
	ImGui::TextWrapped("Object Name");
	ImGui::PushItemWidth(-FLT_MIN);
	ImGui::InputText("", m_szObject, sizeof(m_szObject));

	ImGui::PopItemWidth();
	if (ImGui::Button("Create", { -FLT_MIN, 0 }))
	{
		CreateObject(m_szObject);
	}
	if (ImGui::CollapsingHeader("Debug Toggles"))
	{
		{
			bool& toggle = *(bool*)(_addr(0x1416642C7));
			ImGui::Checkbox("Display Debug Text", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141870449));
			ImGui::Checkbox("Draw Arson Debug Info", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141870447));
			ImGui::Checkbox("Draw Arson Light Visibility", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416642E7));
			ImGui::Checkbox("Draw Blood Particles", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428BA));
			ImGui::Checkbox("Draw Decals", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428BB));
			ImGui::Checkbox("Draw Blood Decals", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416642FC));
			ImGui::Checkbox("Draw Skinned Decals", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428B9));
			ImGui::Checkbox("Draw Screen Decals", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141870443));
			ImGui::Checkbox("Draw Particle Blobs", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141870441));
			ImGui::Checkbox("Draw Particle Info", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141756D97));
			ImGui::Checkbox("Draw Decal Info", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141756DA0));
			ImGui::Checkbox("Draw Effect Stream Info", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141870448));
			ImGui::Checkbox("Use Debug Glass Texture", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416F24ED));
			ImGui::Checkbox("Render clothing memory info", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141713303));
			ImGui::Checkbox("Display World Buffer Info", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428A5));
			ImGui::Checkbox("DrawActors", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428A4));
			ImGui::Checkbox("DrawPlayers", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428A6));
			ImGui::Checkbox("DrawNPCs", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428A7));
			ImGui::Checkbox("DrawSurvivors", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428A8));
			ImGui::Checkbox("DrawBosses", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428A9));
			ImGui::Checkbox("DrawZombies", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428AA));
			ImGui::Checkbox("Draw Dynamic Objects", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428AB));
			ImGui::Checkbox("Draw World", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428AC));
			ImGui::Checkbox("Draw Extra Small Geo", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428AD));
			ImGui::Checkbox("Draw Small Geo", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428AE));
			ImGui::Checkbox("Draw Medium Geo", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428AF));
			ImGui::Checkbox("Draw Large Geo", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428B0));
			ImGui::Checkbox("Draw Huge Geo", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428B1));
			ImGui::Checkbox("Draw Backdrop Geo", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428B2));
			ImGui::Checkbox("Draw Instanced World", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x1416428B3));
			ImGui::Checkbox("Draw Sky", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141870424));
			ImGui::Checkbox("Draw Cam Pos", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141870445));
			ImGui::Checkbox("Draw VFX Names", &toggle);
		}
		{
			bool& toggle = *(bool*)(_addr(0x141870446));
			ImGui::Checkbox("Draw Pyro/Arson Names", &toggle);
		}

		
	}

}

void DR3Menu::DrawMiscTab()
{
	bool& drawfe = *(bool*)(_addr(0x1416642D1));
	ImGui::Checkbox("Draw Frontend/HUD", &drawfe);
	return;

	ImGui::Text("cGame::Instance %p", GetGame());

	static char input[260] = {};
	ImGui::InputText("Name", input, sizeof(input));

	if (ImGui::Button("Spawn"))
		CreateObject(input);

}

void DR3Menu::DrawSettings()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, { 0.5f, 0.5f });
	ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f }, ImGuiCond_Once, { 0.5f, 0.5f });
	ImGui::SetNextWindowSize({ 700,700 }, ImGuiCond_Once);
	ImGui::Begin("Settings", &m_bSubmenuActive[SM_Settings]);

	static int settingID = 0;
	static const char* settingNames[] = {
		"Menu",
		"INI",
		"Keys",
	};

	enum eSettings {
		MENU,
		INI,
		KEYS,
	};

	ImGui::BeginChild("##settings", { 12 * ImGui::GetFontSize(), 0 }, true);

	for (int n = 0; n < IM_ARRAYSIZE(settingNames); n++)
	{
		bool is_selected = (settingID == n);
		if (ImGui::Selectable(settingNames[n], is_selected))
			settingID = n;
		if (is_selected)
			ImGui::SetItemDefaultFocus();
	}

	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("##content", { 0, -ImGui::GetFrameHeightWithSpacing() });

	switch (settingID)
	{
	case MENU:
		ImGui::TextWrapped("All user settings are saved to dr3hook_user.ini.");
		ImGui::Text("Menu Scale");
		ImGui::PushItemWidth(-FLT_MIN);
		ImGui::InputFloat("", &SettingsMgr->fMenuScale);
		ImGui::PopItemWidth();
		break;
	case INI:
		ImGui::TextWrapped("These settings control DR3Hook.ini options. Any changes require game restart to take effect.");
		ImGui::LabelText("", "Core");
		ImGui::Separator();
		ImGui::Checkbox("Debug Console", &SettingsMgr->bEnableConsoleWindow);
		ImGui::Separator();

		break;
	case KEYS:
		if (m_bPressingKey)
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.3f, 1.f), "Press a key!");

		if (ImGui::Button("Reset Keys", { -FLT_MIN, 0 }))
		{
			SettingsMgr->ResetKeys();
			Notifications->SetNotificationTime(2500);
			Notifications->PushNotification("Keys reset! Remember to save.");
		}

		ImGui::Separator();
		ImGui::LabelText("", "Core");
		ImGui::Separator();
		KeyBind(&SettingsMgr->iMenuOpenKey, "Open/Close Menu", "menu");
		KeyBind(&SettingsMgr->iToggleSlowMoKey, "Toggle Slowmotion", "slomo");
		KeyBind(&SettingsMgr->iToggleHUDKey, "Toggle HUD", "hud");
		ImGui::Separator();
		ImGui::LabelText("", "Camera");
		ImGui::Separator();

		KeyBind(&SettingsMgr->iFreeCameraKeyFOVPlus, "FOV+", "fov_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyFOVMinus, "FOV-", "fov_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyYawPlus, "Yaw+", "ya_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYawMinus, "Yaw-", "ya_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyPitchPlus, "Pitch+", "pi_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyPitchMinus, "Pitch-", "pi_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRollPlus, "Roll+", "r_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRollMinus, "Roll-", "r_minus");

		KeyBind(&SettingsMgr->iFreeCameraKeyForward, "Forward", "x_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyBack, "Back", "x_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyLeft, "Left", "y_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyRight, "Right", "y_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyUp, "Up", "z_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyDown, "Down", "z_minus");
		ImGui::Separator();

		if (m_bPressingKey)
		{
			eVKKeyCode result = eKeyboardMan::GetLastKey();

			if (result >= VK_BACKSPACE && result < VK_KEY_NONE)
			{
				*m_pCurrentVarToChange = result;
				m_bPressingKey = false;
			}

		}
		break;
	default:
		break;
	}

	if (ImGui::Button("Save", { -FLT_MIN, 0 }))
	{
		Notifications->SetNotificationTime(2500);
		Notifications->PushNotification("Settings saved to DR3Hook.ini and dr3hook_user.ini!");
		eDirectX11Hook::ms_bShouldReloadFonts = true;
		SettingsMgr->SaveSettings();
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();

	ImGui::End();
}

void DR3Menu::DrawKeyBind(char* name, int* var)
{
	ImGui::SameLine();

	static char butName[256] = {};
	sprintf(butName, "%s##key%s", eKeyboardMan::KeyToString(*var), name);
	if (ImGui::Button(butName))
	{
		m_bPressingKey = true;
		m_pCurrentVarToChange = var;
	}
}

void DR3Menu::KeyBind(int* var, char* bindName, char* name)
{
	ImGui::LabelText("", bindName);
	DrawKeyBind(name, var);
}

void Plugin_UpdateThread()
{
	while (true)
	{
		TheMenu->Update();
		Notifications->Update();
		Sleep(1);
	}
}

float GetDeltaTime()
{
	float delta = 1.0f / 30.0f;

	if (eDirectX11Hook::ms_bInit)
		delta = 1.0f / ImGui::GetIO().Framerate;

	return delta;
}
