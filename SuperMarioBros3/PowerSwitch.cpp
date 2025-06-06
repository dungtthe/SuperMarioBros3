#include "PowerSwitch.h"
int CPowerSwitch::GetIdAnimation() {
	if (isActive) {
		return ID_ANI_POWER_SWITCH_ACTIVE;
	}
	return ID_ANI_POWER_SWITCH_IN_ACTIVE;
}

void CPowerSwitch::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(GetIdAnimation())->Render(x, y);
}

void CPowerSwitch::GetBoundingBox(float& l, float& t, float& r, float& b) {

	float height = POWER_SWITCH_IN_ACTIVE_BBOX_HEIGHT;
	if (isActive) {
		height = POWER_SWITCH_ACTIVE_BBOX_HEIGHT;
	}

	l = x - POWER_SWITCH_BBOX_WIDTH / 2;
	t = y - height / 2;
	r = l + POWER_SWITCH_BBOX_WIDTH;
	b = t + height;
}