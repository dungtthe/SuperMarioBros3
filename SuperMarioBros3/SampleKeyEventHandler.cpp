#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsAutoMove()) {
		return;
	}
	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_RACOON);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_A: {
		mario->Attack();
		break;
	}
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsAutoMove()) {
		return;
	}
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_A:
		mario->SetIsCanHoldingObj(false);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	}

}

void CSampleKeyHandler::KeyState(BYTE* states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->IsAutoMove()) {
		return;
	}
	/*if (game->IsKeyDown(DIK_A)) {
		mario->SetIsCanHoldingObj(true);
	}*/

	if (game->IsKeyDown(DIK_DOWN)) {
		mario->CheckEnterEntrance(DIK_DOWN);
	}
	else if (game->IsKeyDown(DIK_UP)) {
		mario->CheckEnterEntrance(DIK_UP);
	}

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			mario->SetIsCanHoldingObj(true);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
		{
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
			mario->SetIsCanHoldingObj(true);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}