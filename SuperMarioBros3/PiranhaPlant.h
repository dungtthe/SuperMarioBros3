#pragma once
#include "Gameobject.h"
#include <ctime>
#include "Utils.h"



#define PIRANHA_Y_SPEED 0.05f 

//state
#define PIRANHA_STATE_HIDDEN		0
#define PIRANHA_STATE_RISING		1
#define PIRANHA_STATE_CANATTACK		2
#define PIRANHA_STATE_RETRACTING	3
#define PIRANHA_STATE_DIE			4

//PIRANHA TYPE
#define PIRANHAPLANT_RED_HAS_BULLET_TYPE	0
#define PIRANHAPLANT_GREEN_HAS_BULLET_TYPE	1
#define PIRANHAPLANT_GREEN_NO_BULLET_TYPE	2

//RED HAS BULLET
#define PIRANHAPLANT_RED_HAS_BULLET_BBOX_WIDTH		16
#define PIRANHAPLANT_RED_HAS_BULLET_BBOX_HEIGHT		32
//GREEN HAS BULLET
#define PIRANHAPLANT_GREEN_HAS_BULLET_BBOX_WIDTH	16
#define PIRANHAPLANT_GREEN_HAS_BULLET_BBOX_HEIGHT	24
//GREEN NO BULLET
#define PIRANHAPLANT_GREEN_NO_BULLET_BBOX_WIDTH		16
#define PIRANHAPLANT_GREEN_NO_BULLET_BBOX_HEIGHT	24


//RED HAS BULLET
#define ID_ANI_PIRANHAPLANT_RED_HAS_BULLET_LEFT_UP			850500
#define ID_ANI_PIRANHAPLANT_RED_HAS_BULLET_LEFT_DOWN		850501
#define ID_ANI_PIRANHAPLANT_RED_HAS_BULLET_RIGHT_UP			850502
#define ID_ANI_PIRANHAPLANT_RED_HAS_BULLET_RIGHT_DOWN		850503

//GREEN HAS BULLET
#define ID_ANI_PIRANHAPLANT_GREEN_HAS_BULLET_LEFT_UP		850510
#define ID_ANI_PIRANHAPLANT_GREEN_HAS_BULLET_LEFT_DOWN		850511
#define ID_ANI_PIRANHAPLANT_GREEN_HAS_BULLET_RIGHT_UP		850512
#define ID_ANI_PIRANHAPLANT_GREEN_HAS_BULLET_RIGHT_DOWN		850513

//GREEN NO BULLET
#define ID_ANI_PIRANHAPLANT_GREEN_NO_BULLET					850520



#define PIRANHAPLANT_RISING_TIME	4000
#define PIRANHAPLANT_RISING_COOLDOWN_TIME	6000

class CPiranhaPlant : public CGameObject {
private:
	float yHIDDEN;
	float yCANATTACK;
	long startTimeRISING;
	int nxFollow;
	int nyFollow;
	bool canShoot;
	int piranhaType;
	float shootRange;

public:
	CPiranhaPlant(float x, float y, int piranhaType, float shootRange) : CGameObject(x, y)
	{
		this->yHIDDEN = y;

		if (piranhaType == PIRANHAPLANT_RED_HAS_BULLET_TYPE)
		{
			this->yCANATTACK = this->yHIDDEN - PIRANHAPLANT_RED_HAS_BULLET_BBOX_HEIGHT;
		}
		else {
			this->yCANATTACK = this->yHIDDEN - PIRANHAPLANT_GREEN_HAS_BULLET_BBOX_HEIGHT;
		}
		SetState(PIRANHA_STATE_HIDDEN);
		startTimeRISING = 0;
		vy = PIRANHA_Y_SPEED;
		canShoot = true;
		this->piranhaType = piranhaType;
		this->shootRange = shootRange;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void CheckState();
	virtual void FollowMario();
	virtual int GetIdAni();
	virtual int GetBBoxWidth();
	virtual int GetBBoxHeight();
};