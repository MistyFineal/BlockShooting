#include "DxLib.h"
#include "Player.h"
#include "Background.h"
#include "Block.h"
#include "Enemy.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK); //ウィンドウモード変更と初期化と裏画面設定

	/* 初期化 */
	Bg_Ini();
	Block_Ini();
	Enemy_Ini();
	Player_Ini();


	// while(裏画面を表画面に反映, メッセージ処理, 画面クリア)
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		//計算
		Bg_Calc();
		Block_Calc();
		Enemy_Calc();
		Player_Calc();

		//描画
		Bg_Show();
		Block_Show();
		Enemy_Show();
		Player_Show();

		Result_Show();

	}

	DxLib_End();
	return 0;
}
