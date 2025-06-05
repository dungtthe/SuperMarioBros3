#include "RedKoopa.h"
#include "PlayScene.h"
#include "QuestionBlock.h"
#include "PiranhaPlant.h"
int CRedKoopa::GetBBoxHeightCur() {
	if (IsShell() || state == KOOPA_STATE_DIE) {
		return KOOPA_SHELL_BBOX_HEIGHT;
	}
	return KOOPA_RED_BBOX_HEIGHT;
}

int CRedKoopa::GetBBoxWidthCur() {
	if (IsShell() || state == KOOPA_STATE_DIE) {
		return KOOPA_SHELL_BBOX_WIDTH;
	}
	return KOOPA_RED_BBOX_WIDTH;
}

int CRedKoopa::GetIdAnimation()
{
	int aniID = -1;
	if (state == KOOPA_STATE_WALKING_LEFT) {
		aniID = ID_ANI_KOOPA_RED_WALKING_LEFT;
	}
	else if (state == KOOPA_STATE_WALKING_RIGHT) {
		aniID = ID_ANI_KOOPA_RED_WALKING_RIGHT;
	}
	else if (state == KOOPA_STATE_SHELL_IDLE) {
		aniID = ID_ANI_KOOPA_RED_SHELL_IDLE;
	}
	else if (state == KOOPA_STATE_SHELL_REVIVING) {
		aniID = ID_ANI_KOOPA_RED_SHELL_REVIVING;
	}
	else if (state == KOOPA_STATE_SHELL_IDLE_UPTURNED) {
		aniID = ID_ANI_KOOPA_RED_SHELL_IDLE_UPTURNED;
	}
	else if (state == KOOPA_STATE_SHELL_REVIVING_UPTURNED) {
		aniID = ID_ANI_KOOPA_RED_SHELL_REVIVING_UPTURNED;
	}
	else if (state == KOOPA_STATE_SHELL_WALK) {
		aniID = ID_ANI_KOOPA_RED_SHELL_WALK;
	}
	else if (state == KOOPA_STATE_SHELL_WALK_UPTURNED) {
		aniID = ID_ANI_KOOPA_RED_SHELL_WALK_UPTURNED;
	}
	else if (state == KOOPA_STATE_DIE) {
		aniID = ID_ANI_KOOPA_RED_DIE;
	}
	return aniID;
}

float CRedKoopa::GetHeightAdjust()
{
	return (KOOPA_RED_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT) / 2;
}


void CRedKoopa::OnNoCollision(DWORD dt)
{

	if (CGameObject::CheckFallDeath() && state != KOOPA_STATE_DIE) {
		SetState(KOOPA_STATE_DIE);
		DebugOut(L"set koopa die trong CheckFallDeath \n");
		return;
	}

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

		if (deltaX > KOOPA_RED_BBOX_WIDTH / 2 || deltaY > KOOPA_RED_BBOX_HEIGHT / 2) {
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



void CRedKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
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

	CKoopa::OnCollisionWith(e);
}