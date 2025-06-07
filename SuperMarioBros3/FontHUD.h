#pragma once
#include <string>
#include "Sprite.h"
#include <vector>

using namespace std;

#define ID_SPRITE_FONT_HUD_WITDH	8

#define ID_SPRITE_FONT_0	1000500
#define ID_SPRITE_FONT_1	1000501
#define ID_SPRITE_FONT_2	1000502
#define ID_SPRITE_FONT_3	1000503
#define ID_SPRITE_FONT_4	1000504
#define ID_SPRITE_FONT_5	1000505
#define ID_SPRITE_FONT_6	1000506
#define ID_SPRITE_FONT_7	1000507
#define ID_SPRITE_FONT_8	1000508
#define ID_SPRITE_FONT_9	1000509


class CFontHUD {
private:
	float xBegin, y;
	string content;
	vector<CSprite*>sprites;
	void HandleContent();
public:
	CFontHUD(float xBegin, float y, string content) {
		this->xBegin = xBegin;
		this->y = y;
		this->content = content;
		HandleContent();
	}
	void SetContent(string content);
	void Render();
	CSprite *GetSpriteByChar(char c);
};