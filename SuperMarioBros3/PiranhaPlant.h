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



#define PIRANHAPLANT_BBOX_WIDTH 16
#define PIRANHAPLANT_BBOX_HEIGHT 32

#define ID_ANI_PIRANHAPLANT_LEFT_UP 850500
#define ID_ANI_PIRANHAPLANT_LEFT_DOWN 850501
#define ID_ANI_PIRANHAPLANT_RIGHT_UP 850502
#define ID_ANI_PIRANHAPLANT_RIGHT_DOWN 850503


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
public:
	CPiranhaPlant(float x, float y) : CGameObject(x, y)
	{
		this->yHIDDEN = y;
		this->yCANATTACK = this->yHIDDEN - PIRANHAPLANT_BBOX_HEIGHT;
		SetState(PIRANHA_STATE_HIDDEN);
		startTimeRISING = 0;
		vy = PIRANHA_Y_SPEED;
		canShoot = true;
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual int IsCollidable() { return  0; }
	virtual int IsBlocking() { return 0; }
	virtual void SetState(int state);
	virtual void CheckState();
	virtual void FollowMario();
	virtual int GetIdAni();
};