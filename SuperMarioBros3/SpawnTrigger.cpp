#include "SpawnTrigger.h"
#include "Mario.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "RedGoomba.h"

void CSpawnTrigger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario == NULL) {
		return;
	}

	if (!canSpawn || spawnCount >= spawnLimit) {
		return;
	}


	float xMario;
	float yMario;
	mario->GetPosition(xMario, yMario);
	if (xMario >= xSpawnStart && xMario <= xSpawnEnd) {
		CScene* scene = CGame::GetInstance()->GetCurrentScene();
		CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
		if (!playScene) {
			return;
		}
		CGameObject* objSpawn = NULL;
		if (objSpawnType == OBJECT_TYPE_GOOMBA) {
			objSpawn = new CGoomba(xSpawnObj, ySpawnObj);
		}
		else if (objSpawnType == OBJECT_TYPE_RED_GOOMBA) {
			objSpawn = new CRedGoomba(xSpawnObj, ySpawnObj);
		}
		if (objSpawn == NULL) {
			return;
		}
		objSpawn->SetObjectType(objSpawnType);
		objSpawn->SpawnedByTrigger();
		objSpawn->SetIdSpawnTrigger(id);

		//set nx
		float vx;
		float vy;
		objSpawn->GetSpeed(vx, vy);
		objSpawn->SetSpeed(nxStart * abs(vx), vy);

		playScene->AddObject(objSpawn);
		canSpawn = false;

		//count
		if (spawnLimit != -1) {
			spawnCount++;
		}
	}
}

