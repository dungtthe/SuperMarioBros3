#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "BackGroundImage.h"
#include "Pipe.h"
#include "BackGroundTile.h"
#include "SampleKeyEventHandler.h"
#include "QuestionBlock.h"
#include "SpawnTrigger.h"
#include "RedGoomba.h"
#include "Koopa.h"
#include "PiranhaPlant.h"
#include "RedKoopa.h"
#include "GreenKoopa.h"
#include "GoldBrick.h"
#include "ScoreValues.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
	if (hud == NULL) {
		hud = new CHUD();
	}
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());
	if (object_type == OBJECT_TYPE_DEATH_BOUNDARY_Y) {
		deathBoundaryY = y;
		return;
	}
	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x, y); break;
	case OBJECT_TYPE_GOLD_BRICK: {
		bool isRanCoin = atoi(tokens[3].c_str()) == 1;
		bool isRanPowerSwitch = atoi(tokens[4].c_str()) == 1;
		bool isRanMushRoom = atoi(tokens[5].c_str()) == 1;
		obj = new CGoldBrick(x, y, isRanCoin, isRanPowerSwitch, isRanMushRoom);
		break;
	}
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y, COIN_PLACED); break;
	case OBJECT_TYPE_PIRANHAPLANT: {
		int piranhaType = atoi(tokens[3].c_str());
		float shootRange = (float)atof(tokens[4].c_str());
		obj = new CPiranhaPlant(x, y, piranhaType, shootRange);
		break;
	}
	case OBJECT_TYPE_QUESTIONBLOCK: {
		bool isRanCoin = atoi(tokens[3].c_str()) == 1;
		bool isRanRedMushRoom = atoi(tokens[4].c_str()) == 1;
		bool isRanLeaf = atoi(tokens[5].c_str()) == 1;
		obj = new CQuestionBlock(x, y, isRanCoin, isRanRedMushRoom, isRanLeaf);
		break;
	}
	case OBJECT_TYPE_RED_KOOPAS: {
		obj = new CRedKoopa(x, y);
		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		int type = atoi(tokens[9].c_str());
		int shouldCheckDirectionColliable = atoi(tokens[10].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, type, shouldCheckDirectionColliable == 1
		);

		break;
	}
	case OBJECT_TYPE_SPAWNTRIGGER: {

		//x: xStart
		//y: xEnd
		int id = atoi(tokens[3].c_str());
		int spawnLimit = atoi(tokens[4].c_str());
		int objSpawnType = atoi(tokens[5].c_str());

		float xSpawnObj = (float)atof(tokens[6].c_str());
		float ySpawnObj = (float)atof(tokens[7].c_str());
		int nxStart = atoi(tokens[8].c_str());

		vector<string> extraConfig;
		for (size_t i = 9; i < tokens.size(); ++i) {
			extraConfig.push_back(tokens[i]);
		}
		obj = new CSpawnTrigger(id, x, y, spawnLimit, objSpawnType, xSpawnObj, ySpawnObj, nxStart, extraConfig);
		break;
	}

	case OBJECT_TYPE_PIPE: {
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		bool isHasEntrance = ((int)atoi(tokens[9].c_str())) == 1;
		obj = new CPipe(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end,
			isHasEntrance
		);
		if (isHasEntrance) {
			CEntrancePipe* entrancePipe = NULL;

			int entranceType = atoi(tokens[10].c_str());

			float xEntryFirst = (float)atof(tokens[11].c_str());
			float yEntryFirst = (float)atof(tokens[12].c_str());
			float xExitFirst = (float)atof(tokens[13].c_str());
			float yExitFirst = (float)atof(tokens[14].c_str());

			bool isHasTravelSecond = ((int)atoi(tokens[15].c_str())) == 1;
			if (!isHasTravelSecond) {
				entrancePipe = new CEntrancePipe(entranceType, xEntryFirst, yEntryFirst, xExitFirst, yExitFirst, isHasTravelSecond);
			}
			else {
				float xEntrySecond = (float)atof(tokens[16].c_str());
				float yEntrySecond = (float)atof(tokens[17].c_str());
				float xExitSecond = (float)atof(tokens[18].c_str());
				float yExitSecond = (float)atof(tokens[19].c_str());

				entrancePipe = new CEntrancePipe(entranceType, 
					xEntryFirst, yEntryFirst, xExitFirst, yExitFirst, 
					isHasTravelSecond,
					xEntrySecond, yEntrySecond, xExitSecond, yExitSecond
					);
			}
			((CPipe*)obj)->SetEntrancePipe(entrancePipe);
		}
		break;
	}


	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}
	case OBJECT_TYPE_BACKGROUND_IMAGE: {
		int spriteId = atoi(tokens[3].c_str());
		obj = new CBackGroundImage(x, y, spriteId);
		obj->SetObjectType(OBJECT_TYPE_BACKGROUND_IMAGE);
		break;
	}
	case OBJECT_TYPE_BACKGROUND_TILE: {
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = -1;
		int sprite_end = -1;
		if (length > 1) {
			sprite_middle = atoi(tokens[7].c_str());
			sprite_end = atoi(tokens[8].c_str());
		}
		else {
			sprite_middle = sprite_begin;
			sprite_end = sprite_begin;
		}

		obj = new CBackGroundTile(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);
		obj->SetObjectType(OBJECT_TYPE_BACKGROUND_TILE);
		break;
	}

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	obj->SetObjectType(object_type);

	// General object setup
	obj->SetPosition(x, y);

	if (obj->GetObjectType() == OBJECT_TYPE_BACKGROUND_IMAGE) {
		objBackgroundImage = obj;
	}
	else if (obj->GetObjectType() == OBJECT_TYPE_BACKGROUND_TILE) {
		objectsBackGroundTile.push_back(obj);
	}
	else if (obj->GetObjectType() == OBJECT_TYPE_SPAWNTRIGGER) {
		objectsSpawnTrigger.push_back(obj);
	}
	else {
		objects.push_back(obj);
	}
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}


	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	//check SpawnTrigger
	for (int i = 0; i < objectsSpawnTrigger.size(); i++) {
		objectsSpawnTrigger[i]->Update(dt);
	}

	if (hud != NULL) {
		hud->Update();
	}

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetBackBufferWidth() / 2;
	cy -= game->GetBackBufferHeight() / 2;

	if (cx < 0) cx = 0;

	//CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
	CGame::GetInstance()->SetCamPos(cx, cy);

	PurgeDeletedObjects();

	
}

void CPlayScene::Render()
{
	//BG
	//bg img
	if (objBackgroundImage != NULL)
	{
		objBackgroundImage->Render();
	}
	//bg tile
	for (int i = 0; i < objectsBackGroundTile.size(); i++)
	{
		objectsBackGroundTile[i]->Render();
	}


	//OBJ
	/*for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] != player) {
			objects[i]->Render();
		}
	}*/
	vector<LPGAMEOBJECT> bullets;

	CMario* player = dynamic_cast<CMario*>(this->player);
	if (player == NULL) {
		return;
	}
	if (player->IsEnteringPipe()) {
		player->Render();
	}

	vector<LPGAMEOBJECT> objectsNotPlatform;
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetObjectType() == OBJECT_TYPE_BULLET) {
			bullets.push_back(objects[i]);
			continue;
		}
		if (objects[i]->GetObjectType() == OBJECT_TYPE_PLATFORM) {
			objects[i]->Render();
		}
		else {
			objectsNotPlatform.push_back(objects[i]);
		}
	}



	

	vector<LPGAMEOBJECT> objectsPIPE;
	for (int i = 0; i < objectsNotPlatform.size(); i++) {
		if (objectsNotPlatform[i]->GetObjectType() != OBJECT_TYPE_PIPE) {
			if (objectsNotPlatform[i]->GetObjectType() == OBJECT_TYPE_MARIO && player->IsEnteringPipe()) {
				continue;
			}
			objectsNotPlatform[i]->Render();
		}
		else {
			if (objectsNotPlatform[i] != player) {
				objectsPIPE.push_back(objectsNotPlatform[i]);
			}
		}
	}

	for (int i = 0; i < objectsPIPE.size(); i++) {
		objectsPIPE[i]->Render();
	}

	if (!player->IsEnteringPipe()) {
		player->Render();
	}


	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->Render();
	}



	if (hud != NULL) {
		hud->Render();
	}
	objectsNotPlatform.clear();
	objectsPIPE.clear();
	bullets.clear();
}

void CPlayScene::AddObject(LPGAMEOBJECT obj)
{
	objects.push_back(obj);
}

vector<LPGAMEOBJECT> CPlayScene::GetObjectsSpawnTrigger()
{
	return objectsSpawnTrigger;
}

vector<LPGAMEOBJECT> CPlayScene::GetObjectsByType(int objType)
{
	vector<LPGAMEOBJECT> objectsByType;
	for (int i = 0; i < objects.size(); i++) {
		if(objects[i]->GetObjectType() == objType) {
			objectsByType.push_back(objects[i]);
		}
	}
	return objectsByType;
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	for (int i = 0; i < objectsBackGroundTile.size(); i++)
		delete objectsBackGroundTile[i];
	objectsBackGroundTile.clear();
	objBackgroundImage = NULL;
	objectsSpawnTrigger.clear();
	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}