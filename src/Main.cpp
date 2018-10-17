#include "DxLib.h"
#include "Player.h"
#include "Background.h"
#include "Block.h"
#include "Enemy.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK); //�E�B���h�E���[�h�ύX�Ə������Ɨ���ʐݒ�

	/* ������ */
	Bg_Ini();
	Block_Ini();
	Enemy_Ini();
	Player_Ini();


	// while(����ʂ�\��ʂɔ��f, ���b�Z�[�W����, ��ʃN���A)
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		//�v�Z
		Bg_Calc();
		Block_Calc();
		Enemy_Calc();
		Player_Calc();

		//�`��
		Bg_Show();
		Block_Show();
		Enemy_Show();
		Player_Show();

		Result_Show();

	}

	DxLib_End();
	return 0;
}
