#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency

class CGameObject
{
protected:

	float x;
	float y;

	float vx;
	float vy;

	int nx;

	int state;

	bool isDeleted;

	int objType;

	bool isSpawnedByTrigger;
	int idSpawnedByTrigger;

	int score;

public:
	virtual void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }
	virtual void Delete() { isDeleted = true; }
	bool IsDeleted() { return isDeleted; }
	void SetNX(int nx) { this->nx = nx; }
	int GetNX() { return this->nx ; }
	void RenderBoundingBox();
	bool CheckFallDeath();
	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; this->isSpawnedByTrigger = false; this->score = 0; this->objType = -1; }

	int GetScore() { return this->score; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	void SetObjectType(int objType) { this->objType = objType; }
	int GetObjectType() { return objType; }
	void SpawnedByTrigger() { this->isSpawnedByTrigger = true; }
	void SetIdSpawnTrigger(int id) { this->idSpawnedByTrigger = id; }


	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};

	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	// Does this object collide with other object at certain direction ( like ColorBox )
	virtual int IsDirectionColliable(float nx, float ny) { return 1; }

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT& o) { return o->isDeleted; }
};
