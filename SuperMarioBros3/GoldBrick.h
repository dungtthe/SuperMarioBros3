#pragma once
#include "Brick.h"

#define ID_ANI_GOLD_BRICK 900000
#define ID_ANI_GOLD_BRICK_EMPTY 900001

class CGoldBrick : public CBrick {
private:
	bool isRanCoin;
	bool isRanPowerSwitch;
	bool isRanMushRoom;
	bool isEmpty;

public:
	CGoldBrick(float x, float y, bool isRanCoin, bool isRanPowerSwitch, bool isRanMushRoom) : CBrick(x, y) {
		this->isRanCoin = isRanCoin;
		this->isRanPowerSwitch = isRanPowerSwitch;
		this->isRanMushRoom = isRanMushRoom;
		this->isEmpty = false;
	}
	void Update(DWORD dt) {}
	int GetIdAnimation();
	void SpawnItem(bool isSpawnByPowerSwitch);
};