#include "RewardBox.h"
#include "AssetIDs.h"
#include "Mushroom.h"
#include "Utils.h"
void CRewardBox::Render() {

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_REWARD_BOX)->Render(x, y);

	int idAni = GetIdAniItemRan();
	if (idAni == -1) return;

	animations->Get(idAni)->Render(x, y);

}

int CRewardBox::GetIdAniItemRan() {
	int idAni = -1;
	if (typeItemCur == OBJECT_TYPE_MUSHROOM_RED) {
		idAni = ID_ANI_RED_MUSHROOM;
	}
	else if (typeItemCur == OBJECT_TYPE_MUSHROOM_GREEN) {
		idAni = ID_ANI_GREEN_MUSHROOM;
	}
	return idAni;
}
void CRewardBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (objsTypeItemRandom.size() > 1) {
		int indexRan = RandomInRange(0, objsTypeItemRandom.size() - 1);
		if (objsTypeItemRandom.size() > indexRan) {
			typeItemCur = objsTypeItemRandom[indexRan];
		}
	}
}
void CRewardBox::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - REWARD_BOX_BBOX_WIDTH / 2;
	t = y - REWARD_BOX_BBOX_HEIGHT / 2;
	r = l + REWARD_BOX_BBOX_WIDTH;
	b = t + REWARD_BOX_BBOX_HEIGHT;
}
