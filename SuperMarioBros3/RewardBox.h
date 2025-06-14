#pragma once
#include "GameObject.h"

#define REWARD_BOX_BBOX_WIDTH 26
#define REWARD_BOX_BBOX_HEIGHT 26

#define ID_ANI_REWARD_BOX 1100000


class CRewardBox : public CGameObject {
private:
	int typeItemCur;
	vector<int> objsTypeItemRandom; 
	int autoMoveDirX;
public:
	CRewardBox(float x, float y, int autoMoveDirX) :CGameObject(x, y) {
		typeItemCur = -1;
		this->autoMoveDirX = autoMoveDirX;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	void SetObjectTypeRandom(vector<int> objsType) { this->objsTypeItemRandom = objsType; }
	int GetIdAniItemRan();
	int GetAutoMoveDirX() { return autoMoveDirX; }
};