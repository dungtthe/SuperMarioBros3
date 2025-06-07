#include "PointPopup.h"
void CPointPopup::Render() {

	int idAni = GetIdAni();
	if (idAni == -1) {
		return;
	}
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(idAni)->Render(x, y);
}
void  CPointPopup::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	y += vy * dt;
	if ((GetTickCount64() - startTime) > POPUP_TIME_LIVE) {
		isDeleted = true;
	}
}

int CPointPopup::GetBBoxWidthCur() {
	if (popupType == POPUP_LIFE_TYPE) {
		return LIFE_POPUP_BBOX_WIDTH;
	}
	if (score == 100) {
		return SCORE_POPUP_1_0_0_BBOX_WIDTH;
	}
	else if (score == 200) {
		return SCORE_POPUP_2_0_0_BBOX_WIDTH;
	}
	else if (score == 1000) {
		return SCORE_POPUP_1_0_0_0_BBOX_WIDTH;
	}
	return 0;
}

int CPointPopup::GetBBoxHeightCur() {
	if (popupType == POPUP_LIFE_TYPE) {
		return LIFE_POPUP_BBOX_HEIGHT;
	}
	return SCORE_POPUP_BBOX_HEIGHT;
}

int CPointPopup::GetIdAni() {
	if (popupType == POPUP_LIFE_TYPE) {
		return ID_ANI_LIFE_POPUP;
	}
	int idAni = -1;
	if (score == 100) {
		idAni = ID_ANI_SCORE_POPUP_1_0_0;
	}
	else if (score == 200) {
		idAni = ID_ANI_SCORE_POPUP_2_0_0;
	}
	else if (score == 1000) {
		idAni = ID_ANI_SCORE_POPUP_1_0_0_0;
	}
	return idAni;
}

void CPointPopup::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - GetBBoxWidthCur() / 2;
	t = y - GetBBoxHeightCur() / 2;
	r = l + GetBBoxWidthCur();
	b = t + GetBBoxHeightCur();
}