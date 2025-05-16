#include "PiranhaPlant.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Bullet.h"

void CPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PIRANHAPLANT_BBOX_WIDTH / 2;
	top = y - PIRANHAPLANT_BBOX_HEIGHT / 2;
	right = left + PIRANHAPLANT_BBOX_WIDTH;
	bottom = top + PIRANHAPLANT_BBOX_HEIGHT;
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

	if (yMario <= (y + PIRANHAPLANT_BBOX_HEIGHT / 4)) {
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
		playScene->AddObject(new CBullet(x + (nxFollow * PIRANHAPLANT_BBOX_WIDTH) / 2, y - PIRANHAPLANT_BBOX_HEIGHT / 3, nxFollow, nyFollow));
	}
}


int CPiranhaPlant::GetIdAni()
{
	int aniId = -1;
	if (nxFollow >= 0) {
		if (nyFollow >= 0) {
			aniId = ID_ANI_PIRANHAPLANT_RIGHT_UP;
		}
		else {
			aniId = ID_ANI_PIRANHAPLANT_RIGHT_DOWN;
		}
	}
	else {
		if (nyFollow >= 0) {
			aniId = ID_ANI_PIRANHAPLANT_LEFT_UP;
		}
		else {
			aniId = ID_ANI_PIRANHAPLANT_LEFT_DOWN;
		}
	}

	return aniId;
}
