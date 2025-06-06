#include "PowerSwitch.h"
#include "Game.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "GoldBrick.h"
#include "Utils.h"

int CPowerSwitch::GetIdAnimation() {
	if (isActive) {
		return ID_ANI_POWER_SWITCH_ACTIVE;
	}
	return ID_ANI_POWER_SWITCH_IN_ACTIVE;
}

void CPowerSwitch::Render() {
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(GetIdAnimation())->Render(x, y);
}

void CPowerSwitch::GetBoundingBox(float& l, float& t, float& r, float& b) {

	float height = POWER_SWITCH_IN_ACTIVE_BBOX_HEIGHT;
	if (isActive) {
		height = POWER_SWITCH_ACTIVE_BBOX_HEIGHT;
	}

	l = x - POWER_SWITCH_BBOX_WIDTH / 2;
	t = y - height / 2;
	r = l + POWER_SWITCH_BBOX_WIDTH;
	b = t + height;
}

void CPowerSwitch::Active() {
	if (isActive) return; 

	this->isActive = true;
	y += POWER_SWITCH_ACTIVE_HEIGHT_ADJUST + 1;

	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
	if (!playScene) {
		return;
	}

	vector<LPGAMEOBJECT> objects = playScene->GetObjectsByType(OBJECT_TYPE_GOLD_BRICK);
	for(int i =0; i < objects.size(); i++) {
		CGoldBrick* goldBrick = dynamic_cast<CGoldBrick*>(objects[i]);
		if (!goldBrick->IsDeleted()) {
			float goldBrickX, goldBrickY;
			goldBrick->GetPosition(goldBrickX, goldBrickY);

			float distance = calculateDistance(x, y, goldBrickX, goldBrickY);
			if (distance <= POWER_SWITCH_RANGE) {
				goldBrick->SpawnItem(true);
			}
		}
	}
}