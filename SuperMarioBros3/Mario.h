﻿#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"
#include "EntrancePipe.h"
#include "AssetIDs.h"

#define MARIO_GO_ING_ENTRANCE_PIPE_SPEED	0.04f

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f
#define MARIO_FLY_SPEED_Y	0.4f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

#define ID_ANI_MARIO_HOLD_IDLE_RIGHT 1051
#define ID_ANI_MARIO_HOLD_IDLE_LEFT 1052
#define ID_ANI_MARIO_HOLD_WALK_RIGHT 1053
#define ID_ANI_MARIO_HOLD_WALK_LEFT 1054

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_HOLD_IDLE_RIGHT 1701
#define ID_ANI_MARIO_SMALL_HOLD_IDLE_LEFT 1702
#define ID_ANI_MARIO_SMALL_HOLD_WALK_RIGHT 1703
#define ID_ANI_MARIO_SMALL_HOLD_WALK_LEFT 1704

//racoon
#define ID_ANI_MARIO_RACOON_IDLE_RIGHT 2100
#define ID_ANI_MARIO_RACOON_IDLE_LEFT 2102

#define ID_ANI_MARIO_RACOON_WALKING_RIGHT 2200
#define ID_ANI_MARIO_RACOON_WALKING_LEFT 2201

#define ID_ANI_MARIO_RACOON_RUNNING_RIGHT 2300
#define ID_ANI_MARIO_RACOON_RUNNING_LEFT 2301

#define ID_ANI_MARIO_RACOON_JUMP_WALK_RIGHT 2400
#define ID_ANI_MARIO_RACOON_JUMP_WALK_LEFT 2401

#define ID_ANI_MARIO_RACOON_JUMP_RUN_RIGHT 2500
#define ID_ANI_MARIO_RACOON_JUMP_RUN_LEFT 2501

#define ID_ANI_MARIO_RACOON_SIT_RIGHT 2600
#define ID_ANI_MARIO_RACOON_SIT_LEFT 2601

#define ID_ANI_MARIO_RACOON_BRACE_RIGHT 2700
#define ID_ANI_MARIO_RACOON_BRACE_LEFT 2701

#define ID_ANI_MARIO_RACOON_ATTACK_RIGHT 2800
#define ID_ANI_MARIO_RACOON_ATTACK_LEFT 2801

#define ID_ANI_MARIO_RACOON_HOLD_IDLE_RIGHT 2901
#define ID_ANI_MARIO_RACOON_HOLD_IDLE_LEFT 2902

#define ID_ANI_MARIO_RACOON_HOLD_WALK_RIGHT 2903
#define ID_ANI_MARIO_RACOON_HOLD_WALK_LEFT 2904


#pragma endregion




//level
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_RACOON	3


//height k lấy mũ
//big
#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16
#define MARIO_BIG_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)
//small
#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 12
//racoon
#define MARIO_RACOON_BBOX_WIDTH  14
#define MARIO_RACOON_BBOX_HEIGHT 24
#define MARIO_RACOON_SITTING_BBOX_WIDTH  14
#define MARIO_RACOON_SITTING_BBOX_HEIGHT 16
#define MARIO_RACOON_SIT_HEIGHT_ADJUST ((MARIO_RACOON_BBOX_HEIGHT-MARIO_RACOON_SITTING_BBOX_HEIGHT)/2)


#define MARIO_UNTOUCHABLE_TIME 2500
#define MARIO_MAX_FLY_TIME 5000
#define MARIO_RACOON_ATTACK_TIME 300
#define MARIO_RACOON_ATTACK_COOLDOWN 400
#define MARIO_RACOON_FLOATING_TIME 300
//#define MARIO_RACOON_ATTACK_SPEED 0.02f
#define MARIO_RACOON_ATTACK_SPEED 0.08f





class CMario : public CGameObject
{
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin;

	long pCountTimeMeter;
	bool isFlyImmediately;
	long startFlyTime;
	long startAttackTime;
	bool isAttacking;

	bool isCanHoldObj;
	LPGAMEOBJECT objHold;


	int countUntouchable;

	bool isFloating;
	long startFloatingTime;

	bool isEnteringPipe;
	CEntrancePipe* entrancePipe;

	int life;

	int autoMoveDirX;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithGoldBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithPowerSwitch(LPCOLLISIONEVENT e);
	void OnCollisionWithRewardBox(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRacoon();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		//level = MARIO_LEVEL_BIG;
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		pCountTimeMeter = 0;
		isFlyImmediately = false;
		startFlyTime = 0;
		startAttackTime = 0;
		isAttacking = false;
		isCanHoldObj = false;
		objHold = NULL;
		isFloating = false;
		startFloatingTime = 0;
		isEnteringPipe = false;
		objType = OBJECT_TYPE_MARIO;
		life = 4;
		autoMoveDirX = 0;
	}
	void SetPosition(float x, float y);
	void SetSpeed(float vx, float vy);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void UpdateFlyTime(long dt);
	long GetPMeter();
	void Attack();
	void UpdateAttack();
	void UpdateHoldingObj();
	void UpdateCoin(int coinAdd);
	void UpdateScore(int scoreAdd, bool isCanShowScorePopUp = false, float xStartShow = -1, float yStartShow = -1);
	void SetIsCanHoldingObj(bool isCanHold) { this->isCanHoldObj = isCanHold; }
	bool CanHoldObj() { return this->isCanHoldObj; }
	void SetObjHold(LPGAMEOBJECT obj) { this->objHold = obj; }
	int GetLevel() { return this->level; }
	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	int IsBlocking() {return (state != MARIO_STATE_DIE && untouchable == 0);}

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); this->countUntouchable = 0; }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void DecreaseLevel();
	void CheckEnterEntrance(int KeyCode);
	bool IsEnteringPipe() { return this->isEnteringPipe; }
	int GetCoin() { return this->coin; }
	void UpdateLife(int lifeAdd, float xStartShow, float yStartShow);
	int GetLife() { return this->life; }
	bool IsAutoMove() { return this->autoMoveDirX != 0; }
};