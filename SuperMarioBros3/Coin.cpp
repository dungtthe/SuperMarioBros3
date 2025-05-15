#include "Coin.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (coinType != COIN_FROM_QUESTIONBLOCK) {
		return;
	}


	y += -vy * dt;
	vy -= COIN_GRAVITY * dt;

	if (vy < 0 && (yStart - COIN_BBOX_HEIGHT+ COIN_BBOX_HEIGHT/4) <= y) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		mario->UpdateCoint(100);
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
