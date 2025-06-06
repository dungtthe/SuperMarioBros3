#include "GreenKoopa.h"
#include "debug.h"
#include "PlayScene.h"
#include "RedKoopa.h"
#include "QuestionBlock.h"
#include "PiranhaPlant.h"

void CGreenKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (isHasWing && isOnPlatform) {
		if ((GetTickCount64() - startFlyTime) > KOOPA_GREEN_FLY_TIMEOUT) {
			vy = -KOOPA_GREEN_FLY_SPEED;
			startFlyTime = GetTickCount64();
		}
	}
	CKoopa::Update(dt, coObjects);
}

int CGreenKoopa::GetBBoxHeightCur() {
	if (IsShell() || state == KOOPA_STATE_DIE) {
		return KOOPA_SHELL_BBOX_HEIGHT;
	}
	return KOOPA_GREEN_BBOX_HEIGHT;
}

int CGreenKoopa::GetBBoxWidthCur() {
	if (IsShell() || state == KOOPA_STATE_DIE) {
		return KOOPA_SHELL_BBOX_WIDTH;
	}
	return KOOPA_GREEN_BBOX_WIDTH;
}

int CGreenKoopa::GetIdAnimation()
{
	int aniID = -1;
	if (state == KOOPA_STATE_WALKING_LEFT) {
		if (isHasWing) {
			aniID = ID_ANI_KOOPA_GREEN_HASWING_WALKING_LEFT;
		}
		else {
			aniID = ID_ANI_KOOPA_GREEN_WALKING_LEFT;
		}
	}
	else if (state == KOOPA_STATE_WALKING_RIGHT) {
		if (isHasWing) {
			aniID = ID_ANI_KOOPA_GREEN_HASWING_WALKING_RIGHT;
		}
		else {
			aniID = ID_ANI_KOOPA_GREEN_WALKING_RIGHT;
		}
	}
	else if (state == KOOPA_STATE_SHELL_IDLE) {
		aniID = ID_ANI_KOOPA_GREEN_SHELL_IDLE;
	}
	else if (state == KOOPA_STATE_SHELL_REVIVING) {
		aniID = ID_ANI_KOOPA_GREEN_SHELL_REVIVING;
	}
	else if (state == KOOPA_STATE_SHELL_IDLE_UPTURNED) {
		aniID = ID_ANI_KOOPA_GREEN_SHELL_IDLE_UPTURNED;
	}
	else if (state == KOOPA_STATE_SHELL_REVIVING_UPTURNED) {
		aniID = ID_ANI_KOOPA_GREEN_SHELL_REVIVING_UPTURNED;
	}
	else if (state == KOOPA_STATE_SHELL_WALK) {
		aniID = ID_ANI_KOOPA_GREEN_SHELL_WALK;
	}
	else if (state == KOOPA_STATE_SHELL_WALK_UPTURNED) {
		aniID = ID_ANI_KOOPA_GREEN_SHELL_WALK_UPTURNED;
	}
	else if (state == KOOPA_STATE_DIE) {
		aniID = ID_ANI_KOOPA_GREEN_DIE;
	}
	//DebugOut(L"aniid green koopa %d \n", aniID);
	return aniID;
}

float CGreenKoopa::GetHeightAdjust()
{
	return (KOOPA_GREEN_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT) / 2;
}



void CGreenKoopa::OnNoCollision(DWORD dt)
{
	if (isBeingHeld) {
		return;
	}
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
	if (CGameObject::CheckFallDeath() && state != KOOPA_STATE_DIE) {
		this->score = 0;
		SetState(KOOPA_STATE_DIE);
		DebugOut(L"set green koopa die trong CheckFallDeath \n");
		return;
	}
}



void CGreenKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (state == KOOPA_STATE_DIE) {
		return;
	}

	if (!e->obj->IsBlocking()) return;
	if (!isBeingHeld) {
		if (e->ny != 0 && e->obj->IsBlocking())
		{
			vy = 0;
			isOnPlatform = true;
		}
		else if (e->nx != 0 && !dynamic_cast<CGoomba*>(e->obj))
		{
			vx = -vx;
			if (!IsShell()) {
				if (vx > 0) {
					SetState(KOOPA_STATE_WALKING_RIGHT);
				}
				else {
					SetState(KOOPA_STATE_WALKING_LEFT);
				}
			}

		}
	}

	CKoopa::OnCollisionWith(e);
}