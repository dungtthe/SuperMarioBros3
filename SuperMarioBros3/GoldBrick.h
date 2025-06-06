#pragma once
#include "Brick.h"

#define ID_ANI_GOLD_BRICK 900000

class CGoldBrick : public CBrick {
public:
	CGoldBrick(float x, float y) : CBrick(x, y) {}
	void Update(DWORD dt){}
	int GetIdAnimation() { return ID_ANI_GOLD_BRICK; }
};