#include "DxLib.h"
#include "Block.h"
#include "Player.h"
#include "Background.h"

#define COUNTOF(array) (sizeof(array) / sizeof(array[0]))	//配列の要素数求めるマクロ

const int P_SIZE = PLAYER_SIZE;

const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 3;


class Block {
private:
	int BlockImgH;
	const int BLOCK_SIZE = 40;		//ブロックのサイズ
	int x, y;
	int hp;
	bool isDead;
	int BDestructSoundH;		//ブロック破壊音ハンドル

public:
	Block(int n) {
		BlockImgH  = LoadGraph("img/block3.png");		//ブロック画像をロード
		BDestructSoundH = LoadSoundMem("sound/block_dest.mp3");		//ダメージ音をロード
		hp = 3;
		isDead = false;
		if (n < 16) {
			x = BLOCK_SIZE / 2 + n * BLOCK_SIZE;
			y = BLOCK_SIZE / 2;
		}
		else if (n < 26) {
			x = BLOCK_SIZE / 2 + 15 * BLOCK_SIZE;
			y = BLOCK_SIZE / 2 + (n-15) * BLOCK_SIZE;
		}
		else if (n < 42) {
			x = BLOCK_SIZE / 2 + (41-n) * BLOCK_SIZE;
			y = BLOCK_SIZE / 2 +  11 * BLOCK_SIZE;
		}
		else {
			x = BLOCK_SIZE / 2;
			y = BLOCK_SIZE / 2 + (52-n) * BLOCK_SIZE;
		}
	}

	int GetX() {
		return x;
	}
	int GetY() {
		return y;
	}

	bool GetIsDead() {
		return isDead;
	}


	void Show() {
		DrawRotaGraph(x, y, 0.9, 0.0, BlockImgH, TRUE);	
	}

	void Revive() {
		isDead = false;
		hp = 3;
	}

	void CollisionBullet() {		//弾との衝突判定
		int BulletNum = BULLET_NUM;
		for (int i = 0; i < BulletNum; i++) {
			int bx = GetBulletX(i);
			int by = GetBulletY(i);
			bool Bullet_isDead = GetBulletIsDead(i);
			if (!Bullet_isDead) {		//弾が死んでいなければ
				if ((x - bx)*(x - bx) < (BLOCK_SIZE / 2)*(BLOCK_SIZE / 2)) {			//当たり判定
					if ((y - by)*(y - by) < (BLOCK_SIZE / 2)*(BLOCK_SIZE / 2)) {
						KillBullet(i);			//衝突した弾を消す
						hp--;						//hpを-1
						if (hp <= 0) {
							isDead = true;		//hpが0になったら消す
							PlaySoundMem(BDestructSoundH, DX_PLAYTYPE_BACK);
							PointPlus();			//スコア+10pts
						}
					}
				}
			}
		}
	}

	void CollisionPlayer() {			//上方向のプレイヤー衝突判定
		int px = GetPlayerX();
		int py = GetPlayerY();
		if ((x - px) * (x - px) < (BLOCK_SIZE / 2 + P_SIZE/2) * (BLOCK_SIZE / 2 + P_SIZE / 2) &&
			(y - py) * (y - py) < (BLOCK_SIZE / 2 + P_SIZE / 2) * (BLOCK_SIZE / 2 + P_SIZE / 2)) {
			if (GetPlayerIsMoving(UP))		CollisionP_Up();
			if (GetPlayerIsMoving(RIGHT))		CollisionP_Right();
			if (GetPlayerIsMoving(DOWN))		CollisionP_Down();
			if (GetPlayerIsMoving(LEFT))		CollisionP_Left();
		}
	}

};

const int BNUM = BLOCK_NUM;		//ブロックの総数
Block *blocks[BNUM];

int GetBlockX(int n) {
	return blocks[n]->GetX();
}
int GetBlockY(int n) {
	return blocks[n]->GetY();
}
bool GetBlockIsDead(int n) {
	return blocks[n]->GetIsDead();
}
void ReviveBlock(int n) {
	blocks[n]->Revive();
}


void Block_Ini() {
	for (int i = 0; i < COUNTOF(blocks); i++) {
		blocks[i] = new Block(i);
	}
}

void Block_Calc() {
	bool pMovableUp = true;
	bool pMovableRight = true;
	bool pMovableDown = true;
	bool pMovableLeft = true;

	for (int i = 0; i < COUNTOF(blocks); i++) {
		if (!blocks[i]->GetIsDead()) {
			blocks[i]->CollisionBullet();
			blocks[i]->CollisionPlayer();
		}
	}
}

void Block_Show() {
	for (int i = 0; i < COUNTOF(blocks); i++) {
		if (!blocks[i]->GetIsDead())		blocks[i] -> Show();
	}
}