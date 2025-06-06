#include "Goomba.h"
#include "PlayScene.h"
#include "SpawnTrigger.h"
#include "ScoreValues.h"
CGoomba::CGoomba(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
	this->isKilledByTailOrKoopaShell = false;
	this->score = SCORE_VALUE_GOOMBA;
}


void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

	if (CGameObject::CheckFallDeath() && state != GOOMBA_STATE_DIE) {
		this->score = 0;
		SetState(GOOMBA_STATE_DIE);
		DebugOut(L"set goomba die trong CheckFallDeath %f, %f \n",x,y);
	}
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	//if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
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
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
	{
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario != NULL) {
			mario->UpdateScore(this->score, true, x, y);
		}
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		if (!isKilledByTailOrKoopaShell) {
			vx = 0;
			vy = 0;
			ay = 0;
		}
		else {
			vy = -GOOMBA_JUMP_DEFLECT_SPEED;
			vx = -vx;
		}
		break;
	}

	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}

void CGoomba::KilledByTailOrKoopaShell()
{
	this->isKilledByTailOrKoopaShell = true;
}
