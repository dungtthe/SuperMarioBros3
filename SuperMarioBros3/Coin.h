#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "ScoreValues.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16


#define COIN_FLY_SPEED_Y 0.33f
#define COIN_GRAVITY 0.001f


#pragma region coin type
#define	COIN_PLACED 1
#define	COIN_FROM_QUESTIONBLOCK 2
#define	COIN_FROM_GOLDBRICK 3

#pragma endregion


class CCoin : public CGameObject {
private:
	int coinType;
	float yStart;

	//gold brick
	long startSpawnTimeFromGoldBrick;
public:
	CCoin(float x, float y, int coinType, long startSpawnTimeFromGoldBrick = 0) : CGameObject(x, y)
	{
		this->coinType = coinType;
		vy = COIN_FLY_SPEED_Y;
		yStart = y;
		this->score = SCORE_VALUE_COIN;
		this->startSpawnTimeFromGoldBrick = startSpawnTimeFromGoldBrick;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int GetCoinType() { return this->coinType; }
};