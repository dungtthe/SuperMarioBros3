#include "BackGroundImage.h"

void CBackGroundImage::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BACKGROUND_IMG)->Render(x, y);
}

void CBackGroundImage::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	//bg k can
}

