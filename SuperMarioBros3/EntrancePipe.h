#pragma once
#include "GameObject.h"

#define ENTRANCE_MAIN_TO_HIDDEN 0
#define ENTRANCE_HIDDEN_TO_MAIN 1

class CEntrancePipe {
private:
	int entranceType;

	float xEntryFirst, yEntryFirst;
	float xExitFirst, yExitFirst;

	bool isFirstTravelDone;

	bool isHasTravelSecond;

	float xEntrySecond, yEntrySecond;
	float xExitSecond, yExitSecond;

public:
	CEntrancePipe
	(
		int entranceType,
		float xEntryFirst, float yEntryFirst,
		float xExitFirst, float yExitFirst,
		bool isHasTravelSecond = false,
		float xEntrySecond = -1, float yEntrySecond = -1,
		float xExitSecond = -1, float yExitSecond = -1
	)
	{
		this->entranceType = entranceType;

		this->xEntryFirst = xEntryFirst;
		this->yEntryFirst = yEntryFirst;
		this->xExitFirst = xExitFirst;
		this->yExitFirst = yExitFirst;

		this->isHasTravelSecond = isHasTravelSecond;

		this->xEntrySecond = xEntrySecond;
		this->yEntrySecond = yEntrySecond;
		this->xExitSecond = xExitSecond;
		this->yExitSecond = yExitSecond;

		this->isFirstTravelDone = false;
	}

	bool IsFirstTravelDone() { return this->isFirstTravelDone; }
	void DoneFirstTravel() { this->isFirstTravelDone = true; }

	void Reset() { this->isFirstTravelDone = false; }

	int GetEntranceType() { return this->entranceType; }
	float GetXEntryFirst() { return this->xEntryFirst; }
	float GetYEntryFirst() { return this->yEntryFirst; }
	float GetXExitFirst() { return this->xExitFirst; }
	float GetYExitFirst() { return this->yExitFirst; }
	bool IsHasTravelSecond() { return this->isHasTravelSecond; }
	float GetXEntrySecond() { return this->xEntrySecond; }
	float GetYEntrySecond() { return this->yEntrySecond; }
	float GetXExitSecond() { return this->xExitSecond; }
	float GetYExitSecond() { return this->yExitSecond; }
};