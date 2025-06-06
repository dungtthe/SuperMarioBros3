#include "GoldBrick.h"
#include "Mario.h"
#include "PlayScene.h"
#include "ScoreValues.h"
#include "PowerSwitch.h"
#include "Mushroom.h"
#include "Coin.h"
#include "FragmentGoldBrick.h"

int CGoldBrick::GetIdAnimation() {
	if (isEmpty) {
		return ID_ANI_GOLD_BRICK_EMPTY;
	}
	return ID_ANI_GOLD_BRICK;
}

void CGoldBrick::SpawnItem(bool isSpawnByPowerSwitch) {
	if (isEmpty) {
		return;
	}
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
		else if (isRanMushRoom) {
			CScene* scene = CGame::GetInstance()->GetCurrentScene();
			CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
			if (!playScene) {
				return;
			}
			CMushroom* mus = new CMushroom(x, y, MUSHROOM_TYPE_GREEN);
			playScene->AddObject(mus);
			isEmpty = true;
		}

		if (isDeleted) {
			CFragmentGoldBrick* fragment1 = new CFragmentGoldBrick(x, y, 1, 1);
			CFragmentGoldBrick* fragment2 = new CFragmentGoldBrick(x, y, 1, -1);
			CFragmentGoldBrick* fragment3 = new CFragmentGoldBrick(x, y, -1, -1);
			CFragmentGoldBrick* fragment4 = new CFragmentGoldBrick(x, y, -1, 1);
			CScene* scene = CGame::GetInstance()->GetCurrentScene();
			CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
			if (!playScene) {
				return;
			}
			playScene->AddObject(fragment1);
			playScene->AddObject(fragment2);
			playScene->AddObject(fragment3);
			playScene->AddObject(fragment4);
		}
	}
	else {
		if (isRanCoin) {
			CScene* scene = CGame::GetInstance()->GetCurrentScene();
			CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
			if (!playScene) {
				return;
			}
			CCoin* coin = new CCoin(x, y, COIN_FROM_GOLDBRICK, GetTickCount64());
			playScene->AddObject(coin);
			isDeleted = true;
		}
	}
}