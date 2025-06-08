#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"

#include "Collision.h"
#include "QuestionBlock.h"
#include "Koopa.h"
#include "Utils.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "PiranhaPlant.h"
#include "Bullet.h"
#include "RedKoopa.h"
#include "GreenKoopa.h"
#include "GoldBrick.h"
#include "PowerSwitch.h"
#include "Pipe.h"
#include "PlayScene.h"
#include "PointPopup.h"
#include "RedGoomba.h"


void CMario::SetPosition(float x, float y)
{
	if (!isAttacking) {
		this->x = x;
	}
	this->y = y;
}
void CMario::SetSpeed(float vx, float vy) {
	if (isEnteringPipe) {
		return;
	}
	CGameObject::SetSpeed(vx, vy);
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//update entrance pipe
	if (isEnteringPipe) {
		if (entrancePipe == NULL) {
			isEnteringPipe = false;
			return;
		}
		y += vy * dt;
		DebugOut(L"vy: %f, \n", vy);
		if (entrancePipe->GetEntranceType() == ENTRANCE_MAIN_TO_HIDDEN) {
			if (entrancePipe->IsHasTravelSecond()) {
				if (y > entrancePipe->GetYExitSecond()) {
					entrancePipe = NULL;
					isEnteringPipe = false;
				}
				else if (y > entrancePipe->GetYExitFirst() && !entrancePipe->IsFirstTravelDone()) {
					x = entrancePipe->GetXEntrySecond();
					y = entrancePipe->GetYEntrySecond();
					entrancePipe->DoneFirstTravel();
				}
			}
			else {
				if (y > entrancePipe->GetYExitFirst()) {
					entrancePipe = NULL;
					isEnteringPipe = false;
				}
			}
		}
		else {
			if (entrancePipe->IsHasTravelSecond()) {
				if (y < entrancePipe->GetYExitSecond()) {
					entrancePipe = NULL;
					isEnteringPipe = false;
				}
				else if (y < entrancePipe->GetYExitFirst() && !entrancePipe->IsFirstTravelDone()) {
					x = entrancePipe->GetXEntrySecond();
					y = entrancePipe->GetYEntrySecond();
					entrancePipe->DoneFirstTravel();
				}
			}
			else {
				if (y < entrancePipe->GetYExitFirst()) {
					entrancePipe = NULL;
					isEnteringPipe = false;
				}
			}
		}
		return;
	}


	UpdateFlyTime(dt);
	UpdateAttack();


	//check floating
	if (level == MARIO_LEVEL_RACOON) {
		if (!isOnPlatform && GetPMeter() < 6 && isFloating) {
			vy = 0.0f;
		}
		if ((GetTickCount64() - startFloatingTime) > MARIO_RACOON_FLOATING_TIME) {
			isFloating = false;
		}
	}



	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}


	CCollision::GetInstance()->Process(this, dt, coObjects);
	UpdateHoldingObj();
}

void CMario::OnNoCollision(DWORD dt)
{
	if (isEnteringPipe) {
		return;
	}
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
	this->entrancePipe = NULL;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (isEnteringPipe) {
		return;
	}

	if (e->ny != 0 && dynamic_cast<CPipe*>(e->obj))
	{
		/*float pipeX, pipeY;
		e->obj->GetPosition(pipeX, pipeY);
		DebugOut(L"PIPE x: %f, y: %f \n", pipeX, pipeY);*/
		CPipe* pipe = dynamic_cast<CPipe*>(e->obj);
		if (pipe->IsHasEntrance()) {
			this->entrancePipe = pipe->GetEntrancePipe();
			this->entrancePipe->Reset();
		}

	}
	else {
		this->entrancePipe = NULL;
	}

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}

	if (isOnPlatform && e->nx != 0) {
		pCountTimeMeter = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CQuestionBlock*>(e->obj))
		OnCollisionWithQuestionBlock(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	else if (dynamic_cast<CPiranhaPlant*>(e->obj))
		OnCollisionWithPiranhaPlant(e);
	else if (dynamic_cast<CBullet*>(e->obj))
		OnCollisionWithBullet(e);
	else if (dynamic_cast<CGoldBrick*>(e->obj))
		OnCollisionWithGoldBrick(e);
	else if (dynamic_cast<CPowerSwitch*>(e->obj))
		OnCollisionWithPowerSwitch(e);
}


void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mus = dynamic_cast<CMushroom*>(e->obj);
	float xMus, yMus;
	mus->GetPosition(xMus, yMus);
	if (mus->GetTypeMushRoom() == MUSHROOM_TYPE_RED) {
		SetLevel(MARIO_LEVEL_BIG);
		mus->Delete();

		UpdateScore(mus->GetScore(), true, xMus, yMus);
	}
	else {
		DebugOut(L"cong them 1 mang");
		UpdateLife(1, xMus, yMus);
		mus->Delete();
	}
}
void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{
	CLeaf* lea = dynamic_cast<CLeaf*>(e->obj);
	SetLevel(MARIO_LEVEL_RACOON);
	lea->Delete();
	UpdateScore(lea->GetScore(), true, x, y);
}
void CMario::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{
	DebugOut(L"OnCollisionWithPiranhaPlant");
	CPiranhaPlant* piranha = dynamic_cast<CPiranhaPlant*>(e->obj);
	if (piranha->GetState() != PIRANHA_STATE_DIE) {
		if (isAttacking) {
			piranha->SetState(PIRANHA_STATE_DIE);
		}
		else {
			if (untouchable == 0)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					if (level > MARIO_LEVEL_BIG) {
						level = MARIO_LEVEL_BIG;
					}
					else {
						level = MARIO_LEVEL_SMALL;
					}
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithBullet(LPCOLLISIONEVENT e)
{
	CBullet* bullet = dynamic_cast<CBullet*>(e->obj);
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			if (level > MARIO_LEVEL_BIG) {
				level = MARIO_LEVEL_BIG;
			}
			else {
				level = MARIO_LEVEL_SMALL;
			}
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}


void CMario::OnCollisionWithGoldBrick(LPCOLLISIONEVENT e)
{
	CGoldBrick* goldBrick = dynamic_cast<CGoldBrick*>(e->obj);
	if (e->ny > 0 || (e->nx != 0 && isAttacking)) {
		goldBrick->SpawnItem(false);
	}
}

void CMario::OnCollisionWithPowerSwitch(LPCOLLISIONEVENT e)
{
	CPowerSwitch* powerSwitch = dynamic_cast<CPowerSwitch*>(e->obj);
	if (e->ny < 0) {
		if (!powerSwitch->IsActive()) {
			powerSwitch->Active();
		}
	}
}


void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				//check attack
				if (isAttacking) {
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						CRedGoomba* redGoomba = dynamic_cast<CRedGoomba*>(goomba);
						if (redGoomba) {
							if (redGoomba->IsHasWing()) {
								redGoomba->LoseWing();
							}
						}
						goomba->KilledByTailOrKoopaShell();
						goomba->SetState(GOOMBA_STATE_DIE);
					}
				}
				else {
					if (level > MARIO_LEVEL_SMALL)
					{
						if (level > MARIO_LEVEL_BIG) {
							level = MARIO_LEVEL_BIG;
						}
						else {
							level = MARIO_LEVEL_SMALL;
						}
						StartUntouchable();
					}
					else
					{
						DebugOut(L">>> Mario DIE >>> \n");
						SetState(MARIO_STATE_DIE);
					}
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	CCoin* c = (CCoin*)e->obj;
	if (c->GetCoinType() == COIN_FROM_QUESTIONBLOCK) {
		return;
	}
	e->obj->Delete();
	UpdateCoin(1);
	UpdateScore(c->GetScore());
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
	CQuestionBlock* qb = (CQuestionBlock*)e->obj;
	if (e->ny > 0 || (e->nx != 0 && isAttacking)) {
		qb->SpawnItem();
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{

	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	// jump on top >> kill Koopa and deflect a bit 
	if (e->ny < 0)
	{
		if (koopa->GetState() != KOOPA_STATE_DIE)
		{
			if (dynamic_cast<CGreenKoopa*>(e->obj)) {
				CGreenKoopa* greenKoopa = dynamic_cast<CGreenKoopa*>(e->obj);
				if (greenKoopa->IsHasWing()) {
					greenKoopa->SetIsHasWing(false);
					float xGreenK;
					float yGreenK;
					greenKoopa->GetPosition(xGreenK, yGreenK);
					UpdateScore(greenKoopa->GetScore(), true, xGreenK, yGreenK);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
					return;
				}
			}
			if (koopa->IsShell()) {

				if (koopa->GetState() == KOOPA_STATE_SHELL_WALK) {
					koopa->SetState(KOOPA_STATE_SHELL_IDLE);
				}
				else if (koopa->GetState() == KOOPA_STATE_SHELL_WALK_UPTURNED) {
					koopa->SetState(KOOPA_STATE_SHELL_IDLE_UPTURNED);
				}
				else {
					int nxRan = e->nx;
					if (nxRan == 0) {
						nxRan = RandomNV();
					}
					DebugOut(L"jumontop koopa nxran: %d", nxRan);

					koopa->SetNX(nxRan);
					if (koopa->IsShellUpturned()) {
						koopa->SetState(KOOPA_STATE_SHELL_WALK_UPTURNED);
					}
					else {
						koopa->SetState(KOOPA_STATE_SHELL_WALK);
					}
				}

			}
			else {
				koopa->SetState(KOOPA_STATE_SHELL_IDLE);
			}

			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Koopa
	{

		if (untouchable == 0)
		{
			if (koopa->GetState() != KOOPA_STATE_DIE)
			{
				DebugOut(L">>> vao day 1 >>> \n");

				//check hold
				if (isCanHoldObj && koopa->IsShellIdle()) {
					DebugOut(L">>> vao day 2 >>> \n");
					this->objHold = koopa;
				}
				//check attack
				else if (isAttacking) {

					DebugOut(L">>> vao day 3 >>> \n");
					if (dynamic_cast<CGreenKoopa*>(e->obj)) {
						CGreenKoopa* greenKoopa = dynamic_cast<CGreenKoopa*>(e->obj);
						if (greenKoopa->IsHasWing()) {
							greenKoopa->SetIsHasWing(false);
							float xGreenK;
							float yGreenK;
							greenKoopa->GetPosition(xGreenK, yGreenK);
							UpdateScore(greenKoopa->GetScore(), true, xGreenK, yGreenK);
							return;
						}
					}
					koopa->SetState(KOOPA_STATE_SHELL_IDLE_UPTURNED);
				}
				else {
					if (koopa->IsShell() && !koopa->IsShellWalking()) {
						if (e->nx != 0) {
							DebugOut(L">>> vao day 4 >>> \n");
							koopa->SetNX(-e->nx);
							if (koopa->IsShellUpturned()) {
								koopa->SetState(KOOPA_STATE_SHELL_WALK_UPTURNED);
							}
							else {
								koopa->SetState(KOOPA_STATE_SHELL_WALK);
							}
						}
					}
					else {
						DebugOut(L">>> vao day 5 >>> \n");
						if (level > MARIO_LEVEL_SMALL)
						{
							if (level > MARIO_LEVEL_BIG) {
								level = MARIO_LEVEL_BIG;
							}
							else {
								level = MARIO_LEVEL_SMALL;
							}
							StartUntouchable();
						}
						else
						{
							DebugOut(L">>> Mario DIE >>> \n");
							SetState(MARIO_STATE_DIE);
						}
					}

				}
			}
		}
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}

		if (this->isCanHoldObj && objHold != NULL) {
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_SMALL_HOLD_WALK_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_SMALL_HOLD_WALK_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
		{
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;

				if (this->isCanHoldObj && objHold != NULL) {
					if (nx >= 0) {
						aniId = ID_ANI_MARIO_SMALL_HOLD_IDLE_RIGHT;
					}
					else {
						aniId = ID_ANI_MARIO_SMALL_HOLD_IDLE_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

			if (vx != 0) {
				if (this->isCanHoldObj && objHold != NULL) {
					if (nx >= 0) {
						aniId = ID_ANI_MARIO_SMALL_HOLD_WALK_RIGHT;
					}
					else {
						aniId = ID_ANI_MARIO_SMALL_HOLD_WALK_LEFT;
					}
				}
			}
		}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdRacoon()
{
	int aniId = -1;

	if (isAttacking) {
		if (nx >= 0) {
			aniId = ID_ANI_MARIO_RACOON_ATTACK_RIGHT;
		}
		else {
			aniId = ID_ANI_MARIO_RACOON_ATTACK_LEFT;
		}
		return aniId;
	}


	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_JUMP_WALK_LEFT;
		}

		if (this->isCanHoldObj && objHold != NULL) {
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_RACOON_HOLD_WALK_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_RACOON_HOLD_WALK_LEFT;
			}
		}

	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACOON_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACOON_SIT_LEFT;
		}
		else
		{

			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACOON_IDLE_LEFT;

				if (this->isCanHoldObj && objHold != NULL) {
					if (nx >= 0) {
						aniId = ID_ANI_MARIO_RACOON_HOLD_IDLE_RIGHT;
					}
					else {
						aniId = ID_ANI_MARIO_RACOON_HOLD_IDLE_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_RACOON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACOON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACOON_WALKING_LEFT;
			}

			if (vx != 0) {
				if (this->isCanHoldObj && objHold != NULL) {
					if (nx >= 0) {
						aniId = ID_ANI_MARIO_RACOON_HOLD_WALK_RIGHT;
					}
					else {
						aniId = ID_ANI_MARIO_RACOON_HOLD_WALK_LEFT;
					}
				}
			}
		}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACOON_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}

		if (this->isCanHoldObj && objHold != NULL) {
			if (nx >= 0) {
				aniId = ID_ANI_MARIO_HOLD_WALK_RIGHT;
			}
			else {
				aniId = ID_ANI_MARIO_HOLD_WALK_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
		{
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;

				if (this->isCanHoldObj && objHold != NULL) {
					if (nx >= 0) {
						aniId = ID_ANI_MARIO_HOLD_IDLE_RIGHT;
					}
					else {
						aniId = ID_ANI_MARIO_HOLD_IDLE_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

			if (vx != 0) {
				if (this->isCanHoldObj && objHold != NULL) {
					if (nx >= 0) {
						aniId = ID_ANI_MARIO_HOLD_WALK_RIGHT;
					}
					else {
						aniId = ID_ANI_MARIO_HOLD_WALK_LEFT;
					}
				}
			}
		}
	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}




void CMario::Render()
{
	if (untouchable == 1) {
		if (countUntouchable++ < 2) {
			return;
		}
		else {
			countUntouchable = 0;
		}
	}

	//CSprites* sprites = CSprites::GetInstance();
	//sprites->Get(13713)->Draw(x, y);

	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACOON) {
		aniId = GetAniIdRacoon();
	}

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();

	//DebugOutTitle(L"Coins: %d", coin);
	DebugOutTitle(L"X: %.2f, Y: %.2f, P: %d , Coin: %d, isAttacking: %d, isCanHolding: %d, score: %d", x, y, GetPMeter(), coin, isAttacking, isCanHoldObj, score);
}

void CMario::SetState(int state)
{
	if (isEnteringPipe) {
		return;
	}

	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		isFlyImmediately = false;
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		else {
			if (level == MARIO_LEVEL_RACOON) {
				if (GetPMeter() >= 6) {
					isFlyImmediately = true;
					//check thoi gian bay
					if (startFlyTime == 0) {
						startFlyTime = GetTickCount64();
					}
					else {
						if ((GetTickCount64() - startFlyTime) > MARIO_MAX_FLY_TIME) {
							isFlyImmediately = false;
							startFlyTime = 0;
							pCountTimeMeter = 0;
						}
					}
				}
				//floating
				isFloating = true;
				startFloatingTime = GetTickCount64();
			}

		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (isFlyImmediately) {
			vy = -MARIO_FLY_SPEED_Y;
		}
		else {
			if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		}
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y += MARIO_BIG_SIT_HEIGHT_ADJUST;
			isCanHoldObj = false;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_BIG_SIT_HEIGHT_ADJUST + 1;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		isCanHoldObj = false;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::UpdateFlyTime(long dt)
{
	if (!isSitting && (state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_RUNNING_RIGHT)) {
		pCountTimeMeter += dt;
	}
	else {
		pCountTimeMeter -= dt * 2;
	}
	if (pCountTimeMeter < 0) {
		pCountTimeMeter = 0;
	}


	if (GetPMeter() < 6) {
		startFlyTime = 0;
	}
}


long CMario::GetPMeter()
{
	if (pCountTimeMeter <= 0) {
		return 0;
	}

	long pMeter = pCountTimeMeter / 250;
	return pMeter >= 6 ? 6 : pMeter;
}

void CMario::Attack()
{
	if (isSitting) {
		return;
	}

	if (GetTickCount64() - startAttackTime > MARIO_RACOON_ATTACK_COOLDOWN) {
		startAttackTime = GetTickCount64();
		vx += MARIO_RACOON_ATTACK_SPEED * nx;
		isAttacking = true;
	}
}

void CMario::UpdateAttack()
{
	if (level != MARIO_LEVEL_RACOON) {
		isAttacking = false;
		return;
	}
	if ((GetTickCount64() - startAttackTime) > MARIO_RACOON_ATTACK_TIME) {
		isAttacking = false;
	}
	/*else {
		vx += MARIO_RACOON_ATTACK_SPEED * nx;
		isAttacking = true;
	}*/
}

void CMario::UpdateHoldingObj()
{
	if (objHold == NULL || !this->isCanHoldObj) {
		objHold = NULL;
		return;
	}

	CKoopa* koopa = dynamic_cast<CKoopa*>(objHold);
	if (koopa == NULL || koopa->GetState() == KOOPA_STATE_DIE || !koopa->IsShell()) {
		objHold = NULL;
		this->isCanHoldObj = false;
		return;
	}

	float xHold = x + MARIO_SMALL_BBOX_WIDTH * nx;
	float yHold = y;
	if (level == MARIO_LEVEL_BIG) {
		xHold = x + MARIO_BIG_BBOX_WIDTH * nx;
		yHold = y;
	}
	else if (level == MARIO_LEVEL_RACOON) {
		xHold = x + MARIO_RACOON_BBOX_WIDTH * nx;
		yHold = y;
	}
	koopa->SetIsBeingHeld(true);
	koopa->SetPosition(xHold, yHold);
}

void CMario::UpdateCoin(int coinAdd)
{
	coin += coinAdd;
}

void CMario::UpdateScore(int scoreAdd, bool isCanShowScorePopUp, float xStartShow, float yStartShow)
{
	score += scoreAdd;
	if (!isCanShowScorePopUp) {
		return;
	}

	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
	if (!playScene) {
		return;
	}
	CPointPopup* scorePopup = new CPointPopup(xStartShow, yStartShow, scoreAdd);
	playScene->AddObject(scorePopup);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_RACOON) {
		if (isSitting)
		{
			left = x - MARIO_RACOON_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_RACOON_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_RACOON_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_RACOON_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_RACOON_BBOX_WIDTH / 2;
			top = y - MARIO_RACOON_BBOX_HEIGHT / 2;
			right = left + MARIO_RACOON_BBOX_WIDTH;
			bottom = top + MARIO_RACOON_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::DecreaseLevel()
{
	if (level > MARIO_LEVEL_SMALL)
	{
		if (level > MARIO_LEVEL_BIG) {
			level = MARIO_LEVEL_BIG;
		}
		else {
			level = MARIO_LEVEL_SMALL;
		}
		StartUntouchable();
	}
	else
	{
		SetState(MARIO_STATE_DIE);
	}
}

void CMario::CheckEnterEntrance(int KeyCode)
{
	if (entrancePipe == NULL || isEnteringPipe) {
		return;
	}
	isEnteringPipe = true;
	if (KeyCode == DIK_DOWN && entrancePipe->GetEntranceType() == ENTRANCE_MAIN_TO_HIDDEN) {
		DebugOut(L"PIPE_ENTRANCE_MAIN_TO_HIDDEN \n");
		vy = MARIO_GO_ING_ENTRANCE_PIPE_SPEED;
	}
	else if (KeyCode == DIK_UP && entrancePipe->GetEntranceType() == ENTRANCE_HIDDEN_TO_MAIN) {
		DebugOut(L"PIPE_ENTRANCE_HIDDEN_TO_MAIN \n");
		vy = -MARIO_GO_ING_ENTRANCE_PIPE_SPEED;
	}
	x = entrancePipe->GetXEntryFirst();
	y = entrancePipe->GetYEntryFirst();
}

void CMario::UpdateLife(int lifeAdd, float xStartShow, float yStartShow)
{
	life += lifeAdd;
	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
	if (!playScene) {
		return;
	}
	CPointPopup* lifePopup = new CPointPopup(xStartShow, yStartShow, 0, POPUP_LIFE_TYPE);
	playScene->AddObject(lifePopup);
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	else if (this->level == MARIO_LEVEL_BIG && l == MARIO_LEVEL_RACOON) {
		y -= (MARIO_RACOON_BBOX_HEIGHT - MARIO_BIG_BBOX_HEIGHT) / 2;
	}
	/*if ((level - l) != 0) {
		startChangeLevelTime = GetTickCount64();
		countChangeLevel = 0;
	}*/
	level = l;
}

