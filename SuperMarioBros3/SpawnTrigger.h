#pragma once

#include "GameObject.h"
#include "PlayScene.h"
class CSpawnTrigger : public CGameObject {
private:
	int id;//unique
	float xSpawnStart;
	float xSpawnEnd;
	int spawnLimit;
	int spawnCount;
	bool canSpawn;

	//gameobject
	int objSpawnType;
	float xSpawnObj;
	float ySpawnObj;
	int nxStart;

	vector<string> extraConfig;
public:
	CSpawnTrigger(int id, float xSpawnStart, float xSpawnEnd, int spawnLimit, int objSpawnType, float xSpawnObj, float ySpawnObj,int nxStart, vector<string> extraConfig ) {
		this->id = id;
		this->xSpawnStart = xSpawnStart;
		this->xSpawnEnd = xSpawnEnd;
		this->spawnLimit = spawnLimit;
		this->spawnCount = 0;
		this->canSpawn = true;

		this->objSpawnType = objSpawnType;
		this->xSpawnObj = xSpawnObj;
		this->ySpawnObj = ySpawnObj;
		this->nxStart = nxStart;

		this->extraConfig = extraConfig;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render() {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	void CanSpawn() { this->canSpawn = true; }
	int GetID() { return this->id; }
};