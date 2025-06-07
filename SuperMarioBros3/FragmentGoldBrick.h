#pragma once
#include "GameObject.h"

#define FRAGMENT_GRAVITY	0.001f

#define FRAGMENT_BBOX_HEIGHT	8
#define FRAGMENT_BBOX_WIDTH		8

#define ID_ANI_FRAGMENT			900005

#define FRAGMENT_SPEED_X	0.15;
#define FRAGMENT_SPEED_Y	0.2;

#define	FRAGMENT_TIME_LIVE	1000

class CFragmentGoldBrick : public CGameObject {
	long startTime;
	float ay;
	int nySetUp;
public:
	CFragmentGoldBrick(float x, float y, int nx, int ny) : CGameObject(x, y) {
		startTime = GetTickCount64();
		vx = nx * FRAGMENT_SPEED_X;
		vy = ny * FRAGMENT_SPEED_Y;
		if (ny < 0) {
			ay = FRAGMENT_GRAVITY;
		}
		this->nySetUp = ny;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};