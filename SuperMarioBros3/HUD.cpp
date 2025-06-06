#include "HUD.h"
#include "Game.h"

void CHUD::Render() {
	CSprites* cSprites = CSprites::GetInstance();
	CSprite* cSprite = cSprites->Get(ID_SPRITE_HUD_LEFT);
	cSprite->Draw(x, y, false);

	cSprite = cSprites->Get(ID_SPRITE_HUD_RIGHT);
	cSprite->Draw(x + HUD_LEFT_BBOX_WIDTH - 30, y, false);
}

void CHUD::Update() {

}

