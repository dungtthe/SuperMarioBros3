#include "GoldBrick.h"
#include "Mario.h"
#include "PlayScene.h"
#include "ScoreValues.h"
#include "PowerSwitch.h"


int CGoldBrick::GetIdAnimation() {
	if (isEmpty) {
		return ID_ANI_GOLD_BRICK_EMPTY;
	}
	return ID_ANI_GOLD_BRICK;
}

void CGoldBrick::SpawnItem(bool isSpawnByPowerSwitch) {
	if (!isSpawnByPowerSwitch) {
		if (isRanCoin) {
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario != NULL) {
				mario->UpdateScore((int)(SCORE_VALUE_COIN / 10));
			}
			isDeleted = true;
		}
		else if (isRanPowerSwitch) {
			CScene* scene = CGame::GetInstance()->GetCurrentScene();
			CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
			if (!playScene) {
				return;
			}
			CPowerSwitch* powerSwitch = new CPowerSwitch(x, y - BRICK_BBOX_HEIGHT);
			playScene->AddObject(powerSwitch);
			isEmpty = true;
		}
	}
}