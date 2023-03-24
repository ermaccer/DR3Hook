#pragma once
#include "Viewport.h"

class cGame {
public:
	void SetSpeed(float value);
	cViewport* GetViewport(eViewport id);
};

cGame* GetGame();