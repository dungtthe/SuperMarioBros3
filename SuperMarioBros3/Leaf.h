#pragma once
#include "Gameobject.h"
#include <ctime>
#include "Utils.h"
#include "ScoreValues.h"

#define LEAF_GRAVITY 0.000005f
#define LEAF_FLYING_X_SPEED 0.07f 
#define LEAF_PUSHUPFORCE_SPEED 0.2f


#define LEAF_FLYING_X_Change_TIME 1000

#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 14

#define ID_ANI_LEAF 850100

class CLeaf : public CGameObject {
private:
	long startTimeChange;
	float yStart;
public:
	CLeaf(float x, float y) : CGameObject(x, y) {
		nx = RandomNV();
		this->vx = nx * (LEAF_FLYING_X_SPEED / 2.0f);
		//this->vy = -LEAF_PUSHUPFORCE_SPEED;
		this->vy = 0;
		startTimeChange = GetTickCount64();
		this->yStart = yStart;
		this->score = SCORE_VALUE_LEAF;
	}
	int IsBlocking() { return 0; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
};