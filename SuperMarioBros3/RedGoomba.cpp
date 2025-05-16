#include "RedGoomba.h"
#include "SpawnTrigger.h"
#include "PlayScene.h"

void CRedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	FollowMario();

	vy += ay * dt;
	vx += ax * dt;

	//hop
	if (state != GOOMBA_STATE_DIE && isHasWing) {
		long timeCur = GetTickCount64();
		if (timeCur - startTimeHop > RED_GOOMBA_HOP_TIME) {
			isCanHop = true;
			startTimeHop = timeCur;
		}
		else {
			isCanHop = false;
		}

		if (isCanHop) {
			vy = -RED_GOOMBA_HOP_SMALL_SPEED;
			if (hopSmallCount >= RED_GOOMBA_HOP_SMALL_TO_BIG_LIMIT) {
				vy = -RED_GOOMBA_HOP_BIG_SPEED;
				hopSmallCount = 0;
			}
			hopSmallCount++;
		}
	}


	//handle next spawntrigger 
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
				objSpawn->CanSpawn();
			}
		}

		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRedGoomba::Render()
{
	int aniId = ID_ANI_RED_GOOMBA_NOTWING;
	if (state == GOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_RED_GOOMBA_DIE;
	}
	else {
		if (isHasWing) {
			if (isOnPlatform) {
				aniId = ID_ANI_RED_GOOMBA_WING_CLOSE;
			}
			else {
				aniId = ID_ANI_RED_GOOMBA_WING_OPEN;
			}
		}
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}


void CRedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - RED_GOOMBA_BBOX_WIDTH / 2;
		top = y - RED_GOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + RED_GOOMBA_BBOX_WIDTH;
		bottom = top + RED_GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		float height = RED_GOOMBA_BBOX_HEIGHT;
		if (!isHasWing) {
			height = RED_GOOMBA_BBOX_HEIGHT_NOTWING;
		}
		left = x - RED_GOOMBA_BBOX_WIDTH / 2;
		top = y - height / 2;
		right = left + RED_GOOMBA_BBOX_WIDTH;
		bottom = top + height;
	}
}

void CRedGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CRedGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		isOnPlatform = true;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CRedGoomba::FollowMario()
{
	if (!isHasWing) {
		return;
	}

	long timeCur = GetTickCount64();
	if (timeCur - startTimeFollowMario > RED_GOOMBA_FOLLOW_MARIO_TIME) {
		startTimeFollowMario = timeCur;
	}
	else {
		return;
	}

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario == NULL) {
		return;
	}

	float xMario;
	float yMario;
	mario->GetPosition(xMario, yMario);
	if (xMario < x) {
		vx = -abs(vx);
	}
	else {
		vx = abs(vx);
	}
}

void CRedGoomba::SetState(int state)
{
	if (state == GOOMBA_STATE_DIE) {
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario != NULL) {
			mario->UpdateCoint(100);
		}
	}

	if (isHasWing) {
		if (state == GOOMBA_STATE_DIE) {
			isHasWing = false;
			isKilledByTail = false;
			return;
		}
	}

	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (RED_GOOMBA_BBOX_HEIGHT - RED_GOOMBA_BBOX_HEIGHT_DIE) / 2;
		if (!isKilledByTail) {
			vx = 0;
			vy = 0;
			ay = 0;
		}
		else {
			vy = -GOOMBA_JUMP_DEFLECT_SPEED;
			vx = -vx;
		}
		break;
	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}

