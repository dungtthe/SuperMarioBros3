#pragma once
#include "Gameobject.h"
#include <ctime>
#include "Utils.h"
#include "ScoreValues.h"

#define MUSHROOM_TYPE_RED 0
#define MUSHROOM_TYPE_GREEN 1


#define MUSHROOM_GRAVITY 0.002f
#define MUSHROOM_WALKING_SPEED 0.05f
#define MUSHROOM_PUSHUPFORCE_SPEED 0.3f

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 14

#define ID_ANI_RED_MUSHROOM 850000
#define ID_ANI_GREEN_MUSHROOM 850001

class CMushroom :public CGameObject {
private:
	int type;
public:
	CMushroom(float x, float y, int type) :CGameObject(x, y) {
		int nxRan = RandomNV();
		this->vx = nxRan * MUSHROOM_WALKING_SPEED;
		this->vy = -MUSHROOM_PUSHUPFORCE_SPEED;
		this->type = type;

		if (type == MUSHROOM_TYPE_RED) {
			this->score = SCORE_VALUE_MUSHROOM;
		}
	}
	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	int GetTypeMushRoom() { return type; };
};
