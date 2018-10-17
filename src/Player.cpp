#include "DxLib.h"
#include "Player.h"
#include "Enemy.h"
#include "Block.h"
#include "Background.h"

#define PI 3.1415926535897932384626433832795f

const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT= 3;


int Key[256]; // キーが押されているフレーム数を格納する

class Player {
private:
	int PlayerImgH;		//プレイヤー画像ハンドル
	int BulletImgH;		//弾の画像ハンドル
	int x, y;						//プレイヤーのx, y座標
	double angle;			//プレイヤーの角度(描画用)
	int dir;						//プレイヤーの向き(条件分岐用)
	bool isDead;				//プレイヤーの生死フラグ
	bool isNoDamage;	//被弾時の無敵フラグ
	int noDamageCount;  //無敵時間カウント
	int hp;						//プレイヤーHP
	bool isMoving[4];				//移動可能フラグ(0: 上，1: 右，2: 下，3: 左)
	int PBulletSoundH;		//プレイヤーの弾発射音ハンドル
	int PDamageSoundH;		//プレイヤーのダメージ音ハンドル
	int PExplosionSoundH;		//プレイヤーの死亡音ハンドル
	

	static const int BulletNum = BULLET_NUM;		//弾の最大数
	Bullet bullet[BulletNum];		//弾構造体の配列
	int BulletCounter;
	int TimeCounter;

public:
	Player() {
		PlayerImgH = LoadGraph("img/player.png");				//プレイヤー画像をロード
		BulletImgH = LoadGraph("img/pBullet.png");			//弾画像をロード
		PBulletSoundH = LoadSoundMem("sound/player_shot.mp3");		//弾発射音をロード
		PDamageSoundH = LoadSoundMem("sound/player_damage.mp3");		//ダメージ音をロード
		PExplosionSoundH = LoadSoundMem("sound/player_explosion.mp3");		//ダメージ音をロード
		x = 240;
		y = 330;
		angle = 0.0;
		dir = 0;
		isDead = false;
		isNoDamage = false;
		noDamageCount = 0;
		BulletCounter = 0;
		hp = 10;
		isMoving[UP] = false;
		isMoving[RIGHT] = false;
		isMoving[DOWN] = false;
		isMoving[LEFT] = false;
	}

	/*---------------------------プレイヤーの関数----------------------------*/
	
	void Show() {		//プレイヤー描画関数
		if ( !isDead ) {
			//DrawRotaGraph(x, y, 0.6, angle, PlayerImgH, TRUE);		//プレイヤーの描画
			
			if (!isNoDamage) {
				DrawRotaGraph(x, y, 0.6, angle, PlayerImgH, TRUE);		//プレイヤーの描画
			}
			else {
				
				if (noDamageCount % 10 < 5)		DrawRotaGraph(x, y, 0.6, angle, PlayerImgH, TRUE);		//5フレーム毎にプレイヤーの描画
				noDamageCount++;			//無敵カウンタ加算
				
				if (noDamageCount > 60) {	//無敵時間が60フレーム超えたら
					isNoDamage = false;
					noDamageCount = 0;
				}
			}
			
		}
	}

	void Move() {			//プレイヤー移動関数
		//プレイヤー移動
		isMoving[UP] = false;
		isMoving[RIGHT] = false;
		isMoving[DOWN] = false;
		isMoving[LEFT] = false;
		if (Key[KEY_INPUT_W] >= 1 && y > 0) {
			y -= PLAYER_SPEED;
			isMoving[UP] = true;
		}
		if (Key[KEY_INPUT_D] >= 1 && x < 640) {
			x += PLAYER_SPEED;
			isMoving[RIGHT] = true;
		}
		if (Key[KEY_INPUT_S] >= 1 && y < 466) {
			y += PLAYER_SPEED;
			isMoving[DOWN] = true;
		}
		if (Key[KEY_INPUT_A] >= 1 && x > 0) {
			x -= PLAYER_SPEED;
			isMoving[LEFT] = true;
		}

		//向き変更
		if (Key[KEY_INPUT_UP] == 1) {
			angle = 0.0;
			dir = 0;
		} else if  (Key[KEY_INPUT_RIGHT] == 1) {
			angle = PI / 2;
			dir = 1;
		} else if (Key[KEY_INPUT_DOWN] == 1) {
			angle = PI;
			dir = 2;
		} else if (Key[KEY_INPUT_LEFT] == 1) {
			angle = 3 * PI / 2;
			dir = 3;
		}

	}

	int GetPX() {
		return x;
	}
	int GetPY() {
		return y;
	}
	int GetPlayerImgH() {
		return PlayerImgH;
	}
	double GetAngle() {
		return angle;
	}
	bool GetIsDead() {
		return isDead;
	}
	bool GetIsNoDamage() {
		return isNoDamage;
	}
	void Damaged() {
		isNoDamage = true;
		hp--;
		PlaySoundMem(PDamageSoundH, DX_PLAYTYPE_BACK);		//ダメージ音をならす
	}
	void CalcHP() {
		if (hp <= 0) {
			isDead = true;
			//プレイヤーを画面外へ
			x = -40;
			y = -40;
			PlaySoundMem(PExplosionSoundH, DX_PLAYTYPE_BACK);		//死亡したときの爆発音をならす
		}
	}
	int GetHP() {
		return hp;
	}

	void Death() {
		hp = 0;
	}

	void Retry() {
		if (Key[KEY_INPUT_R] >= 1) {
			Player_Ini();
			Enemy_Ini();
			Block_Ini();
			Bg_Ini();
			Key[KEY_INPUT_R] = 0;
		}
	}

	bool GetIsMoving(int n) {
		return isMoving[n];
	}
	void CollisionUp() {
		y += PLAYER_SPEED;
	}
	void CollisionRight() {
		x -= PLAYER_SPEED;
	}
	void CollisionDown() {
		y -= PLAYER_SPEED;
	}
	void CollisionLeft() {
		x += PLAYER_SPEED;
	}



	/*-----------------------------------------------------------------------------*/


	/*---------------------------弾の関数---------------------------------------*/
	void IniBullet() {
		for (int i = 0; i < BulletNum; i++) {
			bullet[i].BulImgH = BulletImgH;
			bullet[i].isDead = true;
			bullet[i].dir = 0;
		}
	}

	Bullet GetBullet(int n) {
		return bullet[n];
	}

	void DeadBullet(int n) {
		bullet[n].isDead = true;
	}

	void Shot() {
		if (Key[KEY_INPUT_LSHIFT] % 10 == 1) {
			bullet[BulletCounter].dir = dir;					//弾の向きをプレイヤーの向きに合わせる
			bullet[BulletCounter].isDead = false;			//弾を可視化
			switch (bullet[BulletCounter].dir) {
			case 0:		//上向き
				bullet[BulletCounter].bx = x;
				bullet[BulletCounter].by = y - 30;
				break;
			case 1:			//右向き
				bullet[BulletCounter].bx = x + 30;
				bullet[BulletCounter].by = y;
				break;
			case 2:		//下向き
				bullet[BulletCounter].bx = x;
				bullet[BulletCounter].by = y + 30;
				break;
			case 3:		//左向き
				bullet[BulletCounter].bx = x - 30;
				bullet[BulletCounter].by = y;
				break;
			default:
				bullet[BulletCounter].isDead = true;
			}
			PlaySoundMem(PBulletSoundH, DX_PLAYTYPE_BACK);
			BulletCounter = (BulletCounter+1) % 30;		//次の弾を装填
		}
	}

	void BulletDraw() {
		for (int i = 0; i < BulletNum; i++) {
			if (!bullet[i].isDead) {
				DrawRotaGraph(bullet[i].bx, bullet[i].by, 0.2, 0.0, bullet[i].BulImgH, TRUE);		//弾を描画
				switch (bullet[i].dir) {
				case 0:		//上向き
					bullet[i].by -= BULLET_SPEED;
					break;
				case 1:			//右向き
					bullet[i].bx += BULLET_SPEED;
					break;
				case 2:		//下向き
					bullet[i].by += BULLET_SPEED;
					break;
				case 3:		//左向き
					bullet[i].bx -= BULLET_SPEED;
					break;
				default:
					bullet[i].isDead = true;
				}

				if (bullet[i].by < -10 || bullet[i].by > 480 || bullet[i].bx < -10 || bullet[i].bx > 650) {
					bullet[i].isDead = true;
				}
			}
		}
	}

	/*-----------------------------------------------------------------------------*/
};

// キーの入力状態を更新する
int UpdateKey() {
	char tmpKey[256]; // 現在のキーの入力状態を格納する
	GetHitKeyStateAll(tmpKey); // 全てのキーの入力状態を得る
	for (int i = 0; i<256; i++) {
		if (tmpKey[i] != 0) { // i番のキーコードに対応するキーが押されていたら
			Key[i]++;     // 加算
		}
		else {              // 押されていなければ
			Key[i] = 0;   // 0にする
		}
	}
	return 0;
}


Player* p;						//プレイヤー"p"を宣言

int GetBulletX(int n) {		//他のクラスに弾のx座標を渡す
	return p->GetBullet(n).bx;
}
int GetBulletY(int n) {		//他のクラスに弾のy座標を渡す
	return p->GetBullet(n).by;
}
bool GetBulletIsDead(int n) {		//他のクラスに弾の生死を返す
	return p->GetBullet(n).isDead;
}
void KillBullet(int n) {
	p->DeadBullet(n);
}
bool GetPlayerIsNoDamage() {
	return p->GetIsNoDamage();
}
void DamagePlayer() {
	p->Damaged();
}
int GetPlayerX() {
	return p->GetPX();
}
int GetPlayerY() {
	return p->GetPY();
}
bool GetPlayerIsMoving(int n) {
	return p->GetIsMoving(n);
}

void CollisionP_Up() {
	p->CollisionUp();
}
void CollisionP_Right() {
	p->CollisionRight();
}
void CollisionP_Down() {
	p->CollisionDown();
}
void CollisionP_Left() {
	p->CollisionLeft();
}

void KillPlayer() {
	p->Death();
}
int GetPlayerHP() {
	return p->GetHP();
}

void Player_Ini() {		//プレイヤー初期化関数
	p = new Player();					//プレイヤーのインスタンスを生成
	p->IniBullet();						//	弾の初期化
}

void Player_Calc() {		//プレイヤーに関する演算
	UpdateKey();
	if (!p->GetIsDead()) {
		p->Move();
		p->Shot();
		p->CalcHP();
	}
	else {
		p->Retry();
	}
}

void Player_Show() {		//プレイヤー描画関数
	p->Show();
	p->BulletDraw();			//プレイヤーの弾の描画

	/*
	ChangeFont("ＭＳ 明朝");
	DrawFormatString(0, 0, GetColor(0, 255, 0), "プレイヤー座標[%d,%d]", p->GetPX(), p->GetPY()); // プレイヤー座標を描画する　※(デバッグ用)
	*/
}