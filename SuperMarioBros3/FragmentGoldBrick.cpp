#include "FragmentGoldBrick.h"
void CFragmentGoldBrick::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_FRAGMENT)->Render(x, y);
}
void CFragmentGoldBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	if (nySetUp < 0) {
		vy += ay * dt;
	}

	y += vy * dt;
	x += vx * dt;
	if ((GetTickCount64() - startTime) > FRAGMENT_TIME_LIVE) {
		isDeleted = true;
	}
}
void CFragmentGoldBrick::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - FRAGMENT_BBOX_WIDTH / 2;
	t = y - FRAGMENT_BBOX_HEIGHT / 2;
	r = l + FRAGMENT_BBOX_WIDTH;
	b = t + FRAGMENT_BBOX_HEIGHT;
}