#include "Koopa.h"
#include "QuestionBlock.h"
#include "Goomba.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"
#include "SpawnTrigger.h"

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - GetBBoxWidthCur() / 2;
	top = y - GetBBoxHeightCur() / 2;
	right = left + GetBBoxWidthCur();
	bottom = top + GetBBoxHeightCur();
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//checkdie
	if (state == KOOPA_STATE_DIE)
	{
		if ((GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT)) {
			CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (mario == NULL) {
				return;
			}
			mario->SetObjHold(NULL);
			if (isSpawnedByTrigger) {
				CScene* scene = CGame::GetInstance()->GetCurrentScene();
				CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
				if (!playScene) {
					return;
				}

				auto objs = playScene->GetObjectsSpawnTrigger();
				for (int i = 0; i < objs.size(); i++) {
					CSpawnTrigger* objSpawn = (CSpawnTrigger*)objs[i];
					if (objSpawn->GetID() == idSpawnedByTrigger) {
						objSpawn->CanSpawn();
					}
				}
			}
			isDeleted = true;
		}
		DebugOut(L"koopa die roi \n");
		return;
	}

	UpdateHeld();

	//update state
	long timeCur = GetTickCount64();
	if ((timeCur - startTime_STATE_SHELL_IDLE) > KOOPA_SHELL_IDLE_TIMEOUT) {
		if (state == KOOPA_STATE_SHELL_IDLE) {
			SetState(KOOPA_STATE_SHELL_REVIVING);
		}
		else if (state == KOOPA_STATE_SHELL_IDLE_UPTURNED) {
			SetState(KOOPA_STATE_SHELL_REVIVING_UPTURNED);
		}
	}
	if ((timeCur - startTime_STATE_SHELL_REVIVING) > KOOPA_SHELL_REVIVING_TIMEOUT) {
		if (state == KOOPA_STATE_SHELL_REVIVING || state == KOOPA_STATE_SHELL_REVIVING_UPTURNED) {
			if (nxPre > 0) {

				SetState(KOOPA_STATE_WALKING_RIGHT);
			}
			else {
				SetState(KOOPA_STATE_WALKING_LEFT);
			}
		}
	}

	if (!isBeingHeld) {
		vy += ay * dt;
		vx += ax * dt;
	}
	//DebugOut(L"vx: %f \n", vx);
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopa::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(GetIdAnimation())->Render(x, y);
}


void CKoopa::OnCollisionWithQestionBlock(LPCOLLISIONEVENT e)
{
	//DebugOut(L">>> KOOPA coli QestionBlock >>> \n");

	CQuestionBlock* qb = (CQuestionBlock*)e->obj;
	if (IsShell() && vx != 0 && !isBeingHeld) {
		qb->SpawnItem();
	}
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	//DebugOut(L">>> KOOPA coli Goomba >>> \n");

	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (this->isBeingHeld) {
		SetState(KOOPA_STATE_DIE);
		vy = -KOOPA_JUMP_DEFLECT_SPEED;
		goomba->KilledByTailOrKoopaShell();
		goomba->SetState(GOOMBA_STATE_DIE);
	}
	else if (IsShell() && vx != 0) {
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->KilledByTailOrKoopaShell();
			goomba->SetState(GOOMBA_STATE_DIE);
		}
	}
}

void CKoopa::SetState(int state)
{
	bool isPreStateShell = IsShell();

	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_DIE: {
		die_start = GetTickCount64();
		break;
	}
	case KOOPA_STATE_WALKING_LEFT:
	{
		vx = -KOOPA_WALKING_SPEED;
		nxPre = -1;
		break;
	}
	case KOOPA_STATE_WALKING_RIGHT:
	{
		vx = KOOPA_WALKING_SPEED;
		nxPre = 1;
		break;
	}
	case KOOPA_STATE_SHELL_IDLE:
	{
		startTime_STATE_SHELL_IDLE = GetTickCount64();
		vx = 0;
		break;
	}
	case KOOPA_STATE_SHELL_REVIVING:
	{
		startTime_STATE_SHELL_REVIVING = GetTickCount64();
		vx = 0;
		break;
	}
	case KOOPA_STATE_SHELL_WALK:
	case KOOPA_STATE_SHELL_WALK_UPTURNED: {
		vx = nx * KOOPA_SHELL_WALKING_SPEED;
		break;
	}
	case KOOPA_STATE_SHELL_IDLE_UPTURNED: {
		startTime_STATE_SHELL_IDLE = GetTickCount64();
		vy = -KOOPA_JUMP_DEFLECT_SPEED;
		vx = 0;
		break;
	}
	case KOOPA_STATE_SHELL_REVIVING_UPTURNED:
	{
		startTime_STATE_SHELL_REVIVING = GetTickCount64();
		vx = 0;
		break;
	}
	}
	if (isPreStateShell && !IsShell()) {

		y -= GetHeightAdjust();
	}


}

void CKoopa::UpdateHeld()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario == NULL) {
		return;
	}
	if (!mario->CanHoldObj()) {
		isBeingHeld = false;
	}

	if (isBeingHeld) {
		if (mario->GetNX() >= 0) {
			vx = KOOPA_SHELL_WALKING_SPEED;
		}
		else {
			vx = -KOOPA_SHELL_WALKING_SPEED;
		}
	}
	//DebugOut(L"vx: %f \n", vx);
	//DebugOut(L"isBeingHeld: %d\n", isBeingHeld);
}
