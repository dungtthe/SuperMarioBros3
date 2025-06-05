#include "PiranhaPlant.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Bullet.h"

void CPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	int width = GetBBoxWidth();
	int height = GetBBoxHeight();
	left = x - width / 2;
	top = y - height / 2;
	right = left + width;
	bottom = top + height;
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	CheckState();
	FollowMario();
}

void CPiranhaPlant::Render()
{
	int aniId = GetIdAni();
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
}


void CPiranhaPlant::SetState(int state)
{
	if (state == PIRANHA_STATE_DIE && this->state == PIRANHA_STATE_DIE) {
		return;
	}

	CGameObject::SetState(state);
	if (state == PIRANHA_STATE_HIDDEN) {
		canShoot = true;
		y = yHIDDEN;
	}
	else if (state == PIRANHA_STATE_CANATTACK) {
		y = yCANATTACK;
	}
	else if (state == PIRANHA_STATE_RISING) {
		vy = -PIRANHA_Y_SPEED;
	}
	else if (state == PIRANHA_STATE_RETRACTING) {
		vy = +PIRANHA_Y_SPEED;
	}
	else if (state == PIRANHA_STATE_DIE) {
		isDeleted = true;
	}
}

void CPiranhaPlant::CheckState()
{
	long timeCur = GetTickCount64();
	if (y >= yHIDDEN) {
		SetState(PIRANHA_STATE_HIDDEN);
		if ((timeCur - startTimeRISING) > PIRANHAPLANT_RISING_COOLDOWN_TIME) {
			startTimeRISING = timeCur;
			SetState(PIRANHA_STATE_RISING);
		}
	}
	else if (y <= yCANATTACK) {
		SetState(PIRANHA_STATE_CANATTACK);
		if ((timeCur - startTimeRISING) > PIRANHAPLANT_RISING_TIME) {
			SetState(PIRANHA_STATE_RETRACTING);
		}
	}
}

void CPiranhaPlant::FollowMario()
{
	if (piranhaType == PIRANHAPLANT_GREEN_NO_BULLET_TYPE) {
		return ;
	}

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario == NULL) {
		return;
	}

	float xMario;
	float yMario;
	mario->GetPosition(xMario, yMario);

	if (xMario <= x) {
		nxFollow = -1;
	}
	else {
		nxFollow = 1;
	}

	if (yMario <= (y + GetBBoxHeight() / 4)) {
		nyFollow = -1;
	}
	else {
		nyFollow = 1;
	}


	//ban
	if (state == PIRANHA_STATE_CANATTACK && canShoot) {
		CScene* scene = CGame::GetInstance()->GetCurrentScene();
		CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
		if (!playScene) {
			return;
		}
		canShoot = false;

		float yBulletOffSet = nyFollow * GetBBoxHeight() / 6.0f;
		playScene->AddObject(new CBullet(x + (nxFollow * GetBBoxWidth()) / 2, y - GetBBoxHeight() / 3 + yBulletOffSet, nxFollow, nyFollow));
	}
}


int CPiranhaPlant::GetIdAni()
{
	if (piranhaType == PIRANHAPLANT_GREEN_NO_BULLET_TYPE) {
		return ID_ANI_PIRANHAPLANT_GREEN_NO_BULLET;
	}

	int aniId = -1;
	if (nxFollow >= 0) {
		if (nyFollow >= 0) {

			if (piranhaType == PIRANHAPLANT_RED_HAS_BULLET_TYPE) {
				aniId = ID_ANI_PIRANHAPLANT_RED_HAS_BULLET_RIGHT_UP;
			}
			else {
				aniId = ID_ANI_PIRANHAPLANT_GREEN_HAS_BULLET_RIGHT_UP;
			}
		}
		else {
			if (piranhaType == PIRANHAPLANT_RED_HAS_BULLET_TYPE) {
				aniId = ID_ANI_PIRANHAPLANT_RED_HAS_BULLET_RIGHT_DOWN;
			}
			else {
				aniId = ID_ANI_PIRANHAPLANT_GREEN_HAS_BULLET_RIGHT_DOWN;
			}
		}
	}
	else {
		if (nyFollow >= 0) {
			if (piranhaType == PIRANHAPLANT_RED_HAS_BULLET_TYPE) {
				aniId = ID_ANI_PIRANHAPLANT_RED_HAS_BULLET_LEFT_UP;
			}
			else {
				aniId = ID_ANI_PIRANHAPLANT_GREEN_HAS_BULLET_LEFT_UP;
			}
		}
		else {
			if (piranhaType == PIRANHAPLANT_RED_HAS_BULLET_TYPE) {
				aniId = ID_ANI_PIRANHAPLANT_RED_HAS_BULLET_LEFT_DOWN;
			}
			else {
				aniId = ID_ANI_PIRANHAPLANT_GREEN_HAS_BULLET_LEFT_DOWN;
			}
		}
	}

	return aniId;
}

int CPiranhaPlant::GetBBoxWidth()
{
	if (piranhaType == PIRANHAPLANT_RED_HAS_BULLET_TYPE) {
		return PIRANHAPLANT_RED_HAS_BULLET_BBOX_WIDTH;
	}
	else if (piranhaType == PIRANHAPLANT_GREEN_NO_BULLET_TYPE) {
		return PIRANHAPLANT_GREEN_NO_BULLET_BBOX_WIDTH;
	}
	return PIRANHAPLANT_GREEN_HAS_BULLET_BBOX_WIDTH;
}

int CPiranhaPlant::GetBBoxHeight()
{
	if (piranhaType == PIRANHAPLANT_RED_HAS_BULLET_TYPE) {
		return PIRANHAPLANT_RED_HAS_BULLET_BBOX_HEIGHT;
	}
	else if (piranhaType == PIRANHAPLANT_GREEN_NO_BULLET_TYPE) {
		return PIRANHAPLANT_GREEN_NO_BULLET_BBOX_HEIGHT;
	}
	return PIRANHAPLANT_GREEN_HAS_BULLET_BBOX_HEIGHT;
}
