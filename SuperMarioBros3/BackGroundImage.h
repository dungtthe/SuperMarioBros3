#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BACKGROUND_IMG 200000

class CBackGroundImage : public CGameObject {
public:
	CBackGroundImage(float x, float y) : CGameObject(x, y) {}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};