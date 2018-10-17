#include "DxLib.h"
#include "Player.h"


int Bg1ImgH;
int Bg2ImgH;
int Bg1Y;
int Bg2Y;

int score;


void Bg_Ini() {
	Bg1ImgH = LoadGraph("img/bg1.png");
	Bg2ImgH = LoadGraph("img/bg2.png");
	Bg1Y = 0;
	Bg2Y = -600;
	score = 0;
}

void Bg_Calc() {
	Bg1Y += 4;
	Bg2Y += 4;
	if (Bg1Y >= 600) {
		Bg1Y = -600;
	}
	else if (Bg2Y >= 600) {
		Bg2Y = -600;
	}
}

void PointPlus() {
	score += 10;
}

void Bg_Show() {
	DrawGraph(0, Bg1Y, Bg1ImgH, false);
	DrawGraph(0, Bg2Y, Bg2ImgH, false);
}

void Result_Show() {			//HPやスコアなど表示
	int pHP = GetPlayerHP();
	SetFontSize(16);
	ChangeFont("Georgia");
	if (pHP >= 7) {		//緑表示
		DrawFormatString(GetPlayerX() - 20, GetPlayerY() - 40, GetColor(0, 255, 0), "HP:%d", GetPlayerHP());
	}
	else if (pHP >= 4) {		//黄色表示
		DrawFormatString(GetPlayerX() - 20, GetPlayerY() - 40, GetColor(255, 255, 0), "HP:%d", GetPlayerHP());
	}
	else if (pHP > 0) {						//赤表示
		DrawFormatString(GetPlayerX() - 20, GetPlayerY() - 40, GetColor(255, 0, 0), "HP:%d", GetPlayerHP());
	}
	else {		//ゲームオーバー表示
		SetFontSize(30);
		DrawFormatString(220, 80, GetColor(255, 0, 0), "GAME OVER...");
		SetFontSize(20);
		DrawFormatString(210, 130, GetColor(0, 255, 0), "Please Press 'R' to Retry" );
	}
	
	SetFontSize(20);
	DrawFormatString(360, 240, GetColor(0, 255, 255), "SCORE: %d", score);
}