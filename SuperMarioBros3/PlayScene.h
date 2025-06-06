#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
//#include "Koopas.h"
#include "HUD.h"

class CPlayScene : public CScene
{
protected:
	float deathBoundaryY;
	long countdownTime;
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;
	CHUD* hud;
	int mapWidth;
	int mapHeight;

	vector<LPGAMEOBJECT> objects;
	LPGAMEOBJECT objBackgroundImage;
	vector<LPGAMEOBJECT> objectsBackGroundTile;
	vector<LPGAMEOBJECT> objectsSpawnTrigger;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	float GetDeathBoundaryY() { return deathBoundaryY; }
	long GetCountdownTime() { return countdownTime; }

	void AddObject(LPGAMEOBJECT obj);
	vector <LPGAMEOBJECT> GetObjectsSpawnTrigger();
	vector <LPGAMEOBJECT> GetObjectsByType(int objType);

	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
};

typedef CPlayScene* LPPLAYSCENE;

