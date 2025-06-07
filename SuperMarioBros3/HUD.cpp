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
			if(coin > 99) {
				coin = 99; 
			}
			fontForCoinMario->SetContent(ConvertNumberToString(coin, 2));

			//score mario
			fontForScoreMario->SetContent(ConvertNumberToString(mario->GetScore(), 7));
		}
	}


}

