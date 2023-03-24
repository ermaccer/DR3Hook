#include <iostream>

#include "..\utils\addr.h"
#include "misc.h"
#include "HashName.h"

void CreateObject(const char* name)
{	
	tHashID hash = 0;
	tHashID* output = nullptr;

	output = cHashName::HashString(&hash, name, strlen(name));


	((void(__fastcall*)(int))_addr(0x140652A40))(*output);

}

void SetControllerMode(int mode, int unk)
{
	((void(__fastcall*)(int, int))_addr(0x140CF7C20))(mode, unk);
}

void SetFormation(int ptr, int unk, int hash)
{

	//static tHashID hashT = 0;
	//
	//if (hashT == 0)
	//	hashT = *cHashName::HashString(&hashT, "hero_nick", strlen("hero_nick"));

	//if (hashT == hash)
		printf("INPUT: %X HASH nick: 7B8918A8\n", hash);

	//hash = 0x19E00E45;
	((void(__fastcall*)(int, int, int))_addr(0x140629800))(ptr, unk, hash);

}
