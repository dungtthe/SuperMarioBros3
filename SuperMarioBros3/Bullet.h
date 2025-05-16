#pragma once
#include "Gameobject.h"
#include <ctime>
#include "Utils.h"



#define BULLET_X_SPEED 0.05f 
#define BULLET_Y_SPEED 0.05f 

#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 8

#define ID_ANI_BULLET 850600

class CBullet : public CGameObject {
public:
	CBullet(float x, float y, int nxFollow, int nyFollow) : CGameObject(x, y)
	{
		if (nxFollow >= 0) {
			vx = BULLET_X_SPEED;
		}
		else {
			vx = -BULLET_X_SPEED;
		}

		if (nyFollow >= 0) {
			vy = BULLET_Y_SPEED;
		}
		else {
			vy = -BULLET_Y_SPEED;
		}
	}
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual int IsCollidable() { return  0; }
	virtual int IsBlocking() { return 0; }
};