#include "Game.h"
#include "..\utils\addr.h"
#include <iostream>


void cGame::SetSpeed(float value)
{
	*(float*)((int64)this + 0xA20) = value;
}

cViewport* cGame::GetViewport(eViewport id)
{
	return 	((cViewport*(__fastcall*)(cGame*, eViewport))_addr(0x14060FD60))(this, id);
}

cGame* GetGame()
{
	if  (*(int64*)(_addr(0x141756C48)))
		return *(cGame**)(_addr(0x141756C48));

	return nullptr;
}
