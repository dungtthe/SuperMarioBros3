#pragma once
#include "GameObject.h"
#include "Game.h"

#define HUD_LEFT_BBOX_WIDTH		152
#define HUD_LEFT_BBOX_HEIGHT	28

#define HUD_RIGHT_BBOX_WIDTH	72
#define HUD_RIGHT_BBOX_HEIGHT	28


#define ID_SPRITE_HUD_LEFT	1000003
#define ID_SPRITE_HUD_RIGHT	1000004

class CHUD  {
private:
	float x, y;
public:
	CHUD() {
		this->x = HUD_LEFT_BBOX_WIDTH / 2 + 30;
		this->y = CGame::GetInstance()->GetBackBufferHeight() - HUD_LEFT_BBOX_HEIGHT/2;
	}
	void Render();
	void Update();
};