#pragma once
#include "Gameobject.h"
#include <ctime>
#include "Utils.h"
#include "ScoreValues.h"

#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_WALKING_SPEED 0.05f
#define MUSHROOM_PUSHUPFORCE_SPEED 0.3f

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 14

#define ID_ANI_RED_MUSHROOM 850000

class CMushroom :public CGameObject {
public:
	CMushroom(float x, float y) :CGameObject(x, y) {
		int nxRan = RandomNV();
		this->vx = nxRan * MUSHROOM_WALKING_SPEED;
		this->vy = -MUSHROOM_PUSHUPFORCE_SPEED;
		this->score = SCORE_VALUE_MUSHROOM;
	}
	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
};
