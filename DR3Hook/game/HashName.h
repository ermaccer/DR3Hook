#pragma once


typedef int tHashID;

class cHashName {
public:
	static tHashID* HashString(tHashID* result, const char* text, int lenght);
};