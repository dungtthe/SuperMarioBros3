#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_QUESTIONBLOCK 100001
#define ID_ANI_QUESTIONBLOCK_EMPTY 100002
#define QUESTIONBLOCK_BBOX_WIDTH 16
#define QUESTIONBLOCK_BBOX_HEIGHT 16

class CQuestionBlock : public CGameObject {
private:
	bool isEmpty;
public:
	CQuestionBlock(float x, float y) : CGameObject(x, y) 
	{
		this->isEmpty = false;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SpawnItem();
};