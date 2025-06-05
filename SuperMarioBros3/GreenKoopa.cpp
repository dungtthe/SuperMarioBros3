#include "GreenKoopa.h"
#include "debug.h"
#include "PlayScene.h"
#include "RedKoopa.h"
#include "QuestionBlock.h"
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
		aniID = ID_ANI_KOOPA_GREEN_WALKING_LEFT;
	}
	else if (state == KOOPA_STATE_WALKING_RIGHT) {
		aniID = ID_ANI_KOOPA_GREEN_WALKING_RIGHT;
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
	if (CGameObject::CheckFallDeath() && state != KOOPA_STATE_DIE) {
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
		if (e->ny != 0)
		{
			vy = 0;
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
	if (dynamic_cast<CQuestionBlock*>(e->obj))
		OnCollisionWithQestionBlock(e);
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
}