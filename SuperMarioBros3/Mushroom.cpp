#include "Mushroom.h"
#include "debug.h"

void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*int nxRan = RandomNV();
	DebugOut(L"nxRan: %d \n", nxRan);*/

	vy += MUSHROOM_GRAVITY * dt;
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::OnNoCollision(DWORD dt)
{
	y += vy * dt;
	x += vx * dt;
};

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushroom::Render()
{
	CAnimations::GetInstance()->Get(ID_ANI_RED_MUSHROOM)->Render(x, y);
}