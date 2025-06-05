#pragma once
#include "Koopa.h"

#define KOOPA_GREEN_BBOX_WIDTH 16
#define KOOPA_GREEN_BBOX_HEIGHT 24


//ani
#define ID_ANI_KOOPA_GREEN_WALKING_LEFT 750050
#define ID_ANI_KOOPA_GREEN_WALKING_RIGHT 750051
#define ID_ANI_KOOPA_GREEN_SHELL_IDLE 750052
#define ID_ANI_KOOPA_GREEN_SHELL_REVIVING 750053
#define ID_ANI_KOOPA_GREEN_SHELL_IDLE_UPTURNED 750054
#define ID_ANI_KOOPA_GREEN_SHELL_REVIVING_UPTURNED 750055
#define ID_ANI_KOOPA_GREEN_SHELL_WALK 750056
#define ID_ANI_KOOPA_GREEN_SHELL_WALK_UPTURNED 750057
#define ID_ANI_KOOPA_GREEN_DIE 750058


class CGreenKoopa : public CKoopa
{
private:
	int GetBBoxWidthCur();
	int GetBBoxHeightCur();
	int GetIdAnimation();
	float GetHeightAdjust();
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CGreenKoopa(float x, float y) :CKoopa(x, y){ }
};