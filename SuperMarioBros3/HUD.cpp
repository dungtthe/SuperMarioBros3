#include "HUD.h"
#include "Game.h"
#include "PlayScene.h"
#include "Utils.h"

void CHUD::Render() {
	CSprites* cSprites = CSprites::GetInstance();
	CSprite* cSprite = cSprites->Get(ID_SPRITE_HUD_LEFT);
	cSprite->Draw(x, y, false);

	cSprite = cSprites->Get(ID_SPRITE_HUD_RIGHT);
	cSprite->Draw(x + HUD_LEFT_BBOX_WIDTH - 30, y, false);



	fontForCountdownTime->Render();
	fontForCoinMario->Render();
	fontForScoreMario->Render();
	fontForLifeMario->Render();


	//p meter mario
	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
	if (playScene) {
		CMario* mario = (CMario*)((LPPLAYSCENE)playScene)->GetPlayer();
		if (mario) {
			int pMeter = mario->GetPMeter();
			if (pMeter < 0) {
				pMeter = 0;
			}
			else if (pMeter >= 6) {
				pMeter = 6;
				cSprite = cSprites->Get(ID_SPRITE_PMETER_FULL);
				cSprite->Draw(this->x - 20 + 7 * PMETER_BBOX_HEIGHT - 3, this->y - 3, false);
			}
			cSprite = cSprites->Get(ID_SPRITE_PMETER);
			for (int i = 0; i < pMeter; i++) {
				cSprite->Draw(this->x - 20 + i * PMETER_BBOX_HEIGHT, this->y - 3, false);
			}
		}
	}
}

void CHUD::Update() {
	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	CPlayScene* playScene = dynamic_cast<CPlayScene*>(scene);
	if (playScene) {
		//count down time
		long countdownTime = playScene->GetCountdownTime();
		int countdownTimeInSeconds = countdownTime / 1000;
		if (countdownTimeInSeconds < 0) {
			countdownTimeInSeconds = 0;
		}
		fontForCountdownTime->SetContent(ConvertNumberToString(countdownTimeInSeconds, 3));

		CMario* mario = (CMario*)((LPPLAYSCENE)playScene)->GetPlayer();
		if (mario) {
			//coin mario
			int coin = mario->GetCoin();
			if (coin > 99) {
				coin = 99;
			}
			fontForCoinMario->SetContent(ConvertNumberToString(coin, 2));

			//score mario
			fontForScoreMario->SetContent(ConvertNumberToString(mario->GetScore(), 7));

			//life mario
			int life = mario->GetLife();
			if (life < 0) {
				life = 0;
			}
			else if (life > 99) {
				life = 99;
			}
			fontForLifeMario->SetContent(ConvertNumberToString(life, 2));
		}
	}


}

