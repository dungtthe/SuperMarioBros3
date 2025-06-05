#pragma once
#include "Koopa.h"

#define KOOPA_RED_BBOX_WIDTH 16
#define KOOPA_RED_BBOX_HEIGHT 24


//ani
#define ID_ANI_KOOPA_RED_WALKING_LEFT 750000
#define ID_ANI_KOOPA_RED_WALKING_RIGHT 750001
#define ID_ANI_KOOPA_RED_SHELL_IDLE 750002
#define ID_ANI_KOOPA_RED_SHELL_REVIVING 750003
#define ID_ANI_KOOPA_RED_SHELL_IDLE_UPTURNED 750004
#define ID_ANI_KOOPA_RED_SHELL_REVIVING_UPTURNED 750005
#define ID_ANI_KOOPA_RED_SHELL_WALK 750006
#define ID_ANI_KOOPA_RED_SHELL_WALK_UPTURNED 750007
#define ID_ANI_KOOPA_RED_DIE 750008


class CRedKoopa : public CKoopa
{
private:
	bool isOnPlatform;
	float xLastOnPlatform;
	float yLastOnPlatform;


	int GetBBoxWidthCur();
	int GetBBoxHeightCur();
	int GetIdAnimation();
	float GetHeightAdjust();
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CRedKoopa(float x, float y) :CKoopa(x, y)
	{
		this->isOnPlatform = false;
		this->xLastOnPlatform = -1;
	}
};