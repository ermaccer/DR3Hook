#pragma once

enum eControllerMode {
	CM_Play = 1,
	CM_FE = 4
};

void CreateObject(const char* name);
void SetControllerMode(int mode, int unk);
void SetFormation(int ptr, int unk, int hash);
