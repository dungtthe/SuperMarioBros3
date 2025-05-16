#include "Leaf.h"
#include "debug.h"

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (yStart > y) {
        vy += LEAF_GRAVITY * dt;
    }
    else {
        vy += (LEAF_GRAVITY/2) * dt;
    }
    x += vx * dt;
    y += vy * dt;

    long timeCur = GetTickCount64();
    if (timeCur - startTimeChange> LEAF_FLYING_X_Change_TIME) {
        nx = -nx;
        vx = LEAF_FLYING_X_SPEED * nx;
        startTimeChange = timeCur;
    }
}

void CLeaf::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_LEAF)->Render(x, y);
}

void CLeaf::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - LEAF_BBOX_WIDTH / 2;
    t = y - LEAF_BBOX_HEIGHT / 2;
    r = l + LEAF_BBOX_WIDTH;
    b = t + LEAF_BBOX_HEIGHT;
}