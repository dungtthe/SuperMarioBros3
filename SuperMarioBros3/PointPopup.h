#pragma once
#include "GameObject.h"

#define POPUP_SCORE_TYPE	0
#define POPUP_LIFE_TYPE		1



#define SCORE_POPUP_1_0_0_BBOX_WIDTH	11
#define SCORE_POPUP_2_0_0_BBOX_WIDTH	12
#define SCORE_POPUP_1_0_0_0_BBOX_WIDTH	15


#define SCORE_POPUP_BBOX_HEIGHT 7


#define LIFE_POPUP_BBOX_HEIGHT			14
#define LIFE_POPUP_BBOX_WIDTH			21


#define ID_ANI_SCORE_POPUP_1_0_0	1000000
#define ID_ANI_SCORE_POPUP_2_0_0	1000001
#define ID_ANI_SCORE_POPUP_1_0_0_0	1000002

#define ID_ANI_LIFE_POPUP			1000005

#define SCORE_POPUP_SPEED_Y 0.04;

#define POPUP_TIME_LIVE 1100

class CPointPopup : public CGameObject {
	long startTime;
	int popupType;
public:
	CPointPopup(float x, float y, int scoreValue, int popupType = POPUP_SCORE_TYPE) : CGameObject(x, y) {
		startTime = GetTickCount64();
		vy = -SCORE_POPUP_SPEED_Y;
		this->score = scoreValue;
		this->popupType = popupType;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	int GetBBoxWidthCur();
	int GetBBoxHeightCur();
	int GetIdAni();
};