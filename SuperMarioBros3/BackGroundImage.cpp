#include "BackGroundImage.h"

void CBackGroundImage::Render()
{
	CSprites* s = CSprites::GetInstance();
	CSprite* sprite = s->Get(this->spriteId);
	if (sprite == NULL) return;
	s->Get(this->spriteId)->Draw(x, y);
}

void CBackGroundImage::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	//bg k can
}

