#include "Koopa.h"
#include "QuestionBlock.h"
#include "Goomba.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (IsShell() || state == KOOPA_STATE_DIE)
	{
		left = x - KOOPA_SHELL_BBOX_WIDTH / 2;
		top = y - KOOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_SHELL_BBOX_WIDTH;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
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
	int aniID = -1;
	if (state == KOOPA_STATE_WALKING_LEFT) {
		aniID = ID_ANI_KOOPA_WALKING_LEFT;
	}
	else if (state == KOOPA_STATE_WALKING_RIGHT) {
		aniID = ID_ANI_KOOPA_WALKING_RIGHT;
	}
	else if (state == KOOPA_STATE_SHELL_IDLE) {
		aniID = ID_ANI_KOOPA_SHELL_IDLE;
	}
	else if (state == KOOPA_STATE_SHELL_REVIVING) {
		aniID = ID_ANI_KOOPA_SHELL_REVIVING;
	}
	else if (state == KOOPA_STATE_SHELL_IDLE_UPTURNED) {
		aniID = ID_ANI_KOOPA_SHELL_IDLE_UPTURNED;
	}
	else if (state == KOOPA_STATE_SHELL_REVIVING_UPTURNED) {
		aniID = ID_ANI_KOOPA_SHELL_REVIVING_UPTURNED;
	}
	else if (state == KOOPA_STATE_SHELL_WALK) {
		aniID = ID_ANI_KOOPA_SHELL_WALK;
	}
	else if (state == KOOPA_STATE_SHELL_WALK_UPTURNED) {
		aniID = ID_ANI_KOOPA_SHELL_WALK_UPTURNED;
	}
	else if (state == KOOPA_STATE_DIE) {
		aniID = ID_ANI_KOOPA_DIE;
	}
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniID)->Render(x, y);
}

void CKoopa::OnNoCollision(DWORD dt)
{
	if (isBeingHeld) {
		xLastOnPlatform = x;
		yLastOnPlatform = y;
		CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario == NULL) {
			return;
		}
		if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
			yLastOnPlatform -= MARIO_RACOON_SIT_HEIGHT_ADJUST;
		}
		return;
	}
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;

	if (xLastOnPlatform != -1 && !IsShell()) {
		float deltaX = abs(x - xLastOnPlatform);
		float deltaY = abs(y - yLastOnPlatform);
		DebugOut(L"deltaX: %.2f, deltaY: %.2f \n", deltaX, deltaY);

		if (deltaX > KOOPA_BBOX_WIDTH / 2 || deltaY > KOOPA_BBOX_HEIGHT / 2) {
			SetState(KOOPA_STATE_DIE);
			DebugOut(L"set koopa die trong check delta \n");
			return;
		}


		x = xLastOnPlatform;
		y = yLastOnPlatform;


		//vx = -vx;
		if (vx > 0) {
			SetState(KOOPA_STATE_WALKING_LEFT);
		}
		else {
			SetState(KOOPA_STATE_WALKING_RIGHT);
		}
	}

	//DebugOut(L"KOOPA_OnNoCollision: x: %.2f, y: %.2f, vx: %.2f, vy: %.2f \n", x, y, vx, vy);

}

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_DIE) {
		return;
	}

	//DebugOut(L"KOOPA_OnCollisionWith: x: %.2f, y: %.2f, vx: %.2f, vy: %.2f \n", x, y, vx, vy);

	if (!e->obj->IsBlocking()) return;

	if (!isBeingHeld) {
		if (e->ny != 0 && e->obj->IsBlocking())
		{
			vy = 0;
			isOnPlatform = true;
			xLastOnPlatform = x;
			yLastOnPlatform = y;
		}
		else if (e->nx != 0 && !dynamic_cast<CGoomba*>(e->obj))
		{
			vx = -vx;

			if (!IsShell() && state != KOOPA_STATE_DIE) {
				if (vx < 0) {
					SetState(KOOPA_STATE_WALKING_LEFT);
				}
				else {
					SetState(KOOPA_STATE_WALKING_RIGHT);
				}
			}

		}
	}

	if (dynamic_cast<CQuestionBlock*>(e->obj))
		OnCollisionWithQestionBlock(e);
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
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
		y -= KOOPA_HEIGHT_ADJUST;
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
