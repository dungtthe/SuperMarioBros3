﻿#include "Coin.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"
#include "PowerSwitch.h"
#include "GoldBrick.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (coinType == COIN_FROM_GOLDBRICK) {
		if ((GetTickCount64() - startSpawnTimeFromGoldBrick) > POWER_SWITCH_DURATION_TIME) {
			CScene* scene = CGame::GetInstance()->GetCurrentScene();
			CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
			if (!playScene) {
				return;
			}
			CGoldBrick * goldBrick =  new CGoldBrick(x, y, true, false, false);
			playScene->AddObject(goldBrick);
			isDeleted = true;
		}

		return;
	}

	if (coinType != COIN_FROM_QUESTIONBLOCK) {
		return;
	}


	y += -vy * dt;
	vy -= COIN_GRAVITY * dt;

	if (vy < 0 && (yStart - COIN_BBOX_HEIGHT+ COIN_BBOX_HEIGHT/4) <= y) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->UpdateCoin(1);
		mario->UpdateScore(this->score, true, x, yStart - COIN_BBOX_HEIGHT);
		isDeleted = true;
	}
}


void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}
