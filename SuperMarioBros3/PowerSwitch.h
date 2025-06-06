#pragma once
#include "GameObject.h"

#define POWER_SWITCH_BBOX_WIDTH 16


#define POWER_SWITCH_IN_ACTIVE_BBOX_HEIGHT 16
#define POWER_SWITCH_ACTIVE_BBOX_HEIGHT 7
#define POWER_SWITCH_ACTIVE_HEIGHT_ADJUST ((POWER_SWITCH_IN_ACTIVE_BBOX_HEIGHT - POWER_SWITCH_ACTIVE_BBOX_HEIGHT)/2)

#define ID_ANI_POWER_SWITCH_IN_ACTIVE 950000
#define ID_ANI_POWER_SWITCH_ACTIVE 950001


#define POWER_SWITCH_RANGE 300


#define POWER_SWITCH_DURATION_TIME 10000

class CPowerSwitch : public CGameObject {
private:
	bool isActive;
public:
	CPowerSwitch(float x, float y) : CGameObject(x, y) { this->isActive = false; }
	void Update(DWORD dt) {}
	void Render();
	int GetIdAnimation();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool IsActive() { return isActive; }
	void Active();
};