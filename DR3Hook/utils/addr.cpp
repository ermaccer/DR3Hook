#include "..\pch.h"
#include "addr.h"

int64 GetEntryPoint()
{
	static int64 addr = reinterpret_cast<int64>(GetModuleHandle(nullptr));
	return addr;
}

int64 _addr(int64 addr)
{
	return GetEntryPoint() - 0x140000000 + addr;
}
