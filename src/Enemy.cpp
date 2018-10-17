#include "DxLib.h"
#include "Block.h"
#include "Enemy.h"
#include "Player.h"
#include <math.h>

const int BNUM = BLOCK_NUM;		//ブロックの総数
const int EBUL_NUM = EBULLET_NUM;
const int EBUL_SPEED = EBULLET_SPEED;
const int P_SIZE = PLAYER_SIZE;
const int E_SIZE = 40;

class Enemy {
private:
	int x, y;
	int speed;
	double angle;				//回転してる角度
	int dang;						//時計回りなら1, 反時計回りなら-1
	int moveCount;
	int EnemyImgH;			//敵画像ハンドル
	int EBulletImgH;			//敵の弾画像ハンドル
	int bn;							//現在いるブロックの配列添字
	int nextbn;					//次の移動先ブロックの配列添字
	bool isMoving;
	bool isClockwise;		//時計回りかそうでないか
	int EBulletSoundH;	//敵の射撃音ハンドル
	int ECreateSoundH;	//敵のブロック生成音ハンドル
	
	EnemyBullet eBullet[EBUL_NUM];
	int bulletCounter;

public:
	Enemy(int n, bool iniClockwise) {
		speed = ENEMY_SPEED;
		EnemyImgH = LoadGraph("img/enemy1.png");
		EBulletImgH = LoadGraph("img/eBullet.png");
		EBulletSoundH = LoadSoundMem("sound/enemy_shot.mp3");		//射撃音をロード
		ECreateSoundH = LoadSoundMem("sound/enemy_create.mp3");		//ブロック生成音をロード
		bn = n;
		nextbn = n;
		angle = 0;
		dang = 1;
		moveCount = 0;
		x = GetBlockX(n);
		y = GetBlockY(n);
		isClockwise = iniClockwise;
		isMoving = !GetBlockIsDead(n);
		IniBullet();		//弾初期化
		bulletCounter = 0;
	}

	/*---------------- 敵本体の関数 -----------------------*/
	void DecisionBehavior() {		//移動か生成か行動を決める
		if (moveCount == 0) {
			bn = nextbn;
			x = GetBlockX(bn);
			y = GetBlockY(bn);
			if (GetBlockIsDead(bn)) {
				isMoving = false;
			}
			else {
				if (isClockwise) {				//時計回りなら
					nextbn = (bn + 1) % BNUM;			//配列を正の方向にループ移動
					dang = 1;
				}
				else {									//反時計回りなら
					nextbn = (bn + (BNUM - 1)) % BNUM;		//配列を負の方向にループ移動
					dang = -1;
				}
				isMoving = true;
			}
			moveCount++;
		}
	}

	void Move() {		//敵移動関数
		if (isMoving) {
			angle += 0.1 * dang;
			if (moveCount < 20) {
				x += speed * ((GetBlockX(nextbn) - GetBlockX(bn)) / 40);
				y += speed * ((GetBlockY(nextbn) - GetBlockY(bn)) / 40);
				moveCount++;
			}
			else {
				moveCount = 0;
			}
		}
		else {
			if (moveCount < 40) {
				moveCount++;
			}
			else {
				ReviveBlock(bn);		//ブロックを生成する
				PlaySoundMem(ECreateSoundH, DX_PLAYTYPE_BACK);		//ブロック生成音をならす
				moveCount = 0;
			}
		}
	}

	//プレイヤーとの衝突判定(即死)
	void CollisionPlayer() {
		int px = GetPlayerX();
		int py = GetPlayerY();
		if ((x - px) * (x - px) < (E_SIZE / 2 + P_SIZE/2) * (E_SIZE / 2 + P_SIZE/2) && 
			(y - py) * (y - py) < (E_SIZE / 2 + P_SIZE/2) * (E_SIZE / 2 + P_SIZE/2)) {
			KillPlayer();
		}
	}
	
	void Show() {
		DrawRotaGraph(x, y, 1.0, angle, EnemyImgH, TRUE);
	}
	/*---------------- 以上敵本体の関数 -----------------------*/

	/*---------------- 敵の弾用関数 -----------------------*/
	void IniBullet() {		//弾初期化関数
		for (int i = 0; i < EBUL_NUM; i++) {
			eBullet[i].isDead = true;
			eBullet[i].vx = 0;
			eBullet[i].vy = 0;
		}
	}

	void Shot() {		//弾発射関数
		if (moveCount % 4 == 0 && isMoving) {
			eBullet[bulletCounter].isDead = false;
			eBullet[bulletCounter].vx = (int)(cos(angle) * EBUL_SPEED);
			eBullet[bulletCounter].vy = (int)(sin(angle) * EBUL_SPEED);
			eBullet[bulletCounter].bx = x + eBullet[bulletCounter].vx;
			eBullet[bulletCounter].by = y + eBullet[bulletCounter].vy;

			PlaySoundMem(EBulletSoundH, DX_PLAYTYPE_BACK);		//射撃音をならす

			bulletCounter = (bulletCounter + 1) % EBUL_NUM;		//次の弾を装填
		}
	}

	void BulletDraw() {		//弾の描画、当たり判定
		for (int i = 0; i < EBUL_NUM; i++) {
			if (!eBullet[i].isDead) {
				DrawRotaGraph(eBullet[i].bx, eBullet[i].by, 0.2, 0.0, EBulletImgH, TRUE);		//弾を描画
				eBullet[i].bx += eBullet[i].vx;
				eBullet[i].by += eBullet[i].vy;

				if (eBullet[i].by < -10 || eBullet[i].by > 480 || eBullet[i].bx < -10 || eBullet[i].bx > 650) {
					eBullet[i].isDead = true;
				}

				//プレイヤーとの当たり判定
				if (!GetPlayerIsNoDamage()) {
					int px = GetPlayerX();
					int py = GetPlayerY();
					if ((px - eBullet[i].bx)*(px - eBullet[i].bx) < (P_SIZE / 2) * (P_SIZE / 2)) {
						if ((py - eBullet[i].by)*(py - eBullet[i].by) < (P_SIZE / 2) * (P_SIZE / 2)) {
							DamagePlayer();
							eBullet[i].isDead = true;
						}
					}
				}
			}
		}
	}

	/*---------------- 以上敵の弾用関数 ------------------*/
};


Enemy *enemy1, *enemy2;

void Enemy_Ini() {
	enemy1 = new Enemy(0, true);
	enemy2 = new Enemy(26, false);
}

void Enemy_Calc() {
	enemy1->DecisionBehavior();
	enemy2->DecisionBehavior();
	enemy1->Move();
	enemy2->Move();
	enemy1->CollisionPlayer();
	enemy2->CollisionPlayer();
	enemy1->Shot();
	enemy2->Shot();
}

void Enemy_Show() {
	enemy1->Show();
	enemy2->Show();
	enemy1->BulletDraw();
	enemy2->BulletDraw();
}