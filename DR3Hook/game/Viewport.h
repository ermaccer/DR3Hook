#pragma once
#include "..\utils\addr.h"
#include "..\core\Vector.h"
enum eViewport {
	VIEWPORT_MAIN = 11,
};


class cViewport {
public:
	void* vTable;
	int field8;
	int field12;
	int64 field16;
	int field24;
	int field28;
	int64 field32;

	Vector mCameraPos; // umvc3 style naming
	Vector mCameraView;
	Vector mCameraUp;

	float mFOV;

	float field80;
	float field84;
	int	  field88;

	Vector mVec0;

};