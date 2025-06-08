#pragma once

#include "GameObject.h"
#include "Goomba.h"



#define RED_GOOMBA_BBOX_WIDTH 16
#define RED_GOOMBA_BBOX_HEIGHT 16
#define RED_GOOMBA_BBOX_HEIGHT_NOTWING 12
#define RED_GOOMBA_BBOX_HEIGHT_DIE 5

#define ID_ANI_RED_GOOMBA_NOTWING 6000
#define ID_ANI_RED_GOOMBA_WING_OPEN 6001
#define ID_ANI_RED_GOOMBA_WING_CLOSE 6003
#define ID_ANI_RED_GOOMBA_DIE 6002

#define RED_GOOMBA_HOP_SMALL_SPEED  0.06f
#define RED_GOOMBA_HOP_BIG_SPEED  0.28f
#define RED_GOOMBA_HOP_SMALL_TO_BIG_LIMIT  3
#define RED_GOOMBA_HOP_TIME 400

#define RED_GOOMBA_FOLLOW_MARIO_TIME 1500

class CRedGoomba : public CGoomba
{
protected:
	bool isHasWing;
	bool isOnPlatform;
	int hopSmallCount;
	long startTimeHop;
	bool isCanHop;
	long startTimeFollowMario;

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void FollowMario();
public:
	CRedGoomba(float x, float y) :CGoomba(x, y)
	{
		this->isHasWing = true;
		this->isOnPlatform = false;
		this->hopSmallCount = 0;
		this->startTimeHop = 0;
		this->startTimeFollowMario = 0;
		this->isCanHop = true;
	}
	virtual void SetState(int state);
	void LoseWing() { this->isHasWing = false; }
	bool IsHasWing() { return this->isHasWing; }
};
