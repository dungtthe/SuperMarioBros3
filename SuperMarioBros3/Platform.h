#pragma once

#include "GameObject.h"


//PlatformType
#define PLATFORM_TYPE_GROUND	0
#define PLATFORM_TYPE_TEETH		1
#define PLATFORM_TYPE_METAL		2

// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	int type;
	bool shouldCheckDirectionColliable;

public:
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end, int type, bool shouldCheckDirectionColliable) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
		this->type = type;
		this->shouldCheckDirectionColliable = shouldCheckDirectionColliable;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);
};

typedef CPlatform* LPPLATFORM;