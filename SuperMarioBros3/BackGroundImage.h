#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

class CBackGroundImage : public CGameObject {
private:
	int spriteId;
public:
	CBackGroundImage(float x, float y, int spriteId) : CGameObject(x, y) {
		this->spriteId = spriteId;
	}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};