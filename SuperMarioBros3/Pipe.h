#pragma once

#include "GameObject.h"
#include "EntrancePipe.h"



class CPipe : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	bool isHasEntrance;

	CEntrancePipe* entrancePipe;

public:
	CPipe(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end,
		bool isHasEntrance ) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
		this->isHasEntrance = isHasEntrance;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	void SetEntrancePipe(CEntrancePipe* entrancePipe) { this->entrancePipe = entrancePipe; }
	CEntrancePipe* GetEntrancePipe() { return this->entrancePipe; }
	bool IsHasEntrance() { return this->isHasEntrance; }

};
