#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_SHELL_WALKING_SPEED 0.18f

#define KOOPA_JUMP_DEFLECT_SPEED  0.5f



#define KOOPA_SHELL_BBOX_WIDTH 16
#define KOOPA_SHELL_BBOX_HEIGHT 14


#define KOOPA_DIE_TIMEOUT 500

//state
#define KOOPA_STATE_WALKING_LEFT 100
#define KOOPA_STATE_WALKING_RIGHT 200
//shell
#define KOOPA_STATE_SHELL_IDLE 300
#define KOOPA_STATE_SHELL_REVIVING 400
#define KOOPA_STATE_SHELL_IDLE_UPTURNED 350
#define KOOPA_STATE_SHELL_REVIVING_UPTURNED 450
#define KOOPA_STATE_SHELL_WALK 500
#define KOOPA_STATE_SHELL_WALK_UPTURNED 550

#define KOOPA_STATE_DIE 600


//time
#define KOOPA_SHELL_IDLE_TIMEOUT 6000
#define KOOPA_SHELL_REVIVING_TIMEOUT 2500


class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	
	long startTime_STATE_SHELL_IDLE;
	long startTime_STATE_SHELL_REVIVING;
	int nxPre;
	bool isBeingHeld;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return  state != KOOPA_STATE_DIE; }
	virtual int IsBlocking() { return state != KOOPA_STATE_DIE; }
	virtual void OnNoCollision(DWORD dt) = 0;
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) = 0;
	virtual void OnCollisionWithQestionBlock(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	virtual int GetBBoxWidthCur() = 0;
	virtual int GetBBoxHeightCur() = 0;
	virtual int GetIdAnimation() = 0;
	virtual float GetHeightAdjust() { return 0; }

public:
	CKoopa(float x, float y) :CGameObject(x, y)
	{
		this->ax = 0;
		this->ay = KOOPA_GRAVITY;
		die_start = -1;
		SetState(KOOPA_STATE_WALKING_LEFT);
		
		this->isBeingHeld = false;
	}

	virtual void SetState(int state);
	virtual void SetIsBeingHeld(bool isHeld) { this->isBeingHeld = isHeld; };
	virtual void UpdateHeld();
	bool IsShell() {
		return !(state == KOOPA_STATE_WALKING_LEFT || state == KOOPA_STATE_WALKING_RIGHT);
	}
	bool IsShellUpturned() {
		return state == KOOPA_STATE_SHELL_IDLE_UPTURNED || state == KOOPA_STATE_SHELL_REVIVING_UPTURNED;
	}
	bool IsShellWalking() {
		return state == KOOPA_STATE_SHELL_WALK || state == KOOPA_STATE_SHELL_WALK_UPTURNED;
	}
	bool IsShellIdle() {
		return state == KOOPA_STATE_SHELL_IDLE || state == KOOPA_STATE_SHELL_IDLE_UPTURNED;
	}
};