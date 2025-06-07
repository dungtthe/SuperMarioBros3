#include "FontHUD.h"
#include "Sprites.h"
void CFontHUD::Render() {
	if (content == ""){
		return;
	}
	for (int i = 0; i < sprites.size(); i++) {
		CSprite* sprite = sprites[i];
		sprite->Draw(xBegin + ID_SPRITE_FONT_HUD_WITDH * i, y, false);
	}
}

void CFontHUD::HandleContent()
{
	sprites.clear();
	for (char c : content) {
		CSprite* findSprite = GetSpriteByChar(c);
		if (findSprite != NULL) {
			sprites.push_back(findSprite);
		}
	}
}

void CFontHUD::SetContent(string content)
{
	this->content = content;
	HandleContent();
}

CSprite* CFontHUD::GetSpriteByChar(char c)
{
	CSprites* cSprites = CSprites::GetInstance();
	int idSprite = -1;

	switch (c)
	{
	case '0': idSprite = ID_SPRITE_FONT_0; break;
	case '1': idSprite = ID_SPRITE_FONT_1; break;
	case '2': idSprite = ID_SPRITE_FONT_2; break;
	case '3': idSprite = ID_SPRITE_FONT_3; break;
	case '4': idSprite = ID_SPRITE_FONT_4; break;
	case '5': idSprite = ID_SPRITE_FONT_5; break;
	case '6': idSprite = ID_SPRITE_FONT_6; break;
	case '7': idSprite = ID_SPRITE_FONT_7; break;
	case '8': idSprite = ID_SPRITE_FONT_8; break;
	case '9': idSprite = ID_SPRITE_FONT_9; break;
	default:
		return NULL;
	}
	return cSprites->Get(idSprite);
}
