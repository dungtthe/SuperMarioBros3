#pragma once
#include "GameObject.h"
#include "Game.h"
#include "FontHUD.h"

#define HUD_LEFT_BBOX_WIDTH		152
#define HUD_LEFT_BBOX_HEIGHT	28

#define HUD_RIGHT_BBOX_WIDTH	72
#define HUD_RIGHT_BBOX_HEIGHT	28


#define PMETER_BBOX_HEIGHT		8


#define ID_SPRITE_HUD_LEFT	1000003
#define ID_SPRITE_HUD_RIGHT	1000004


#define ID_SPRITE_PMETER		1001000
#define ID_SPRITE_PMETER_FULL	1001001




class CHUD  {
private:
	float x, y;
	CFontHUD* fontForCountdownTime;
	CFontHUD* fontForCoinMario;
	CFontHUD* fontForScoreMario;
	CFontHUD* fontForLifeMario;
public:
	CHUD() {
		this->x = HUD_LEFT_BBOX_WIDTH / 2 + 30;
		this->y = CGame::GetInstance()->GetBackBufferHeight() - HUD_LEFT_BBOX_HEIGHT/2;

		fontForCountdownTime = new CFontHUD(this->x + HUD_LEFT_BBOX_WIDTH / 2 - 22, this->y + 5, "000");
		fontForCoinMario = new CFontHUD(this->x + HUD_LEFT_BBOX_WIDTH / 2 - 16, this->y - 3, "00");
		fontForScoreMario = new CFontHUD(this->x - 20, this->y + 5, "0000000");
		fontForLifeMario = new CFontHUD(this->x - HUD_LEFT_BBOX_WIDTH / 2 + 32, this->y + 5, "00");
	}
	void Render();
	void Update();
};