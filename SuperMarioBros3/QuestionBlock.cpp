#include "QuestionBlock.h"
#include "Coin.h"
#include "Game.h"
#include "PlayScene.h"

void CQuestionBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (isEmpty) {
		animations->Get(ID_ANI_QUESTIONBLOCK_EMPTY)->Render(x, y);
	}
	else {
		animations->Get(ID_ANI_QUESTIONBLOCK)->Render(x, y);
	}
	//RenderBoundingBox();
}

void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - QUESTIONBLOCK_BBOX_WIDTH / 2;
	t = y - QUESTIONBLOCK_BBOX_HEIGHT / 2;
	r = l + QUESTIONBLOCK_BBOX_WIDTH;
	b = t + QUESTIONBLOCK_BBOX_HEIGHT;
}

void CQuestionBlock::SpawnItem(int marioState)
{
	if (isEmpty) {
		return;
	}
	isEmpty = true;
	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
	if (!playScene) {
		return;
	}

	//spawn coin
	CCoin* coin = new CCoin(this->x, this->y, COIN_FROM_QUESTIONBLOCK);
	playScene->AddObject(coin);


}
