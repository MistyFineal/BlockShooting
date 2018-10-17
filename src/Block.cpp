#include "DxLib.h"
#include "Block.h"
#include "Player.h"
#include "Background.h"

#define COUNTOF(array) (sizeof(array) / sizeof(array[0]))	//�z��̗v�f�����߂�}�N��

const int P_SIZE = PLAYER_SIZE;

const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 3;


class Block {
private:
	int BlockImgH;
	const int BLOCK_SIZE = 40;		//�u���b�N�̃T�C�Y
	int x, y;
	int hp;
	bool isDead;
	int BDestructSoundH;		//�u���b�N�j�󉹃n���h��

public:
	Block(int n) {
		BlockImgH  = LoadGraph("img/block3.png");		//�u���b�N�摜�����[�h
		BDestructSoundH = LoadSoundMem("sound/block_dest.mp3");		//�_���[�W�������[�h
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

	void CollisionBullet() {		//�e�Ƃ̏Փ˔���
		int BulletNum = BULLET_NUM;
		for (int i = 0; i < BulletNum; i++) {
			int bx = GetBulletX(i);
			int by = GetBulletY(i);
			bool Bullet_isDead = GetBulletIsDead(i);
			if (!Bullet_isDead) {		//�e������ł��Ȃ����
				if ((x - bx)*(x - bx) < (BLOCK_SIZE / 2)*(BLOCK_SIZE / 2)) {			//�����蔻��
					if ((y - by)*(y - by) < (BLOCK_SIZE / 2)*(BLOCK_SIZE / 2)) {
						KillBullet(i);			//�Փ˂����e������
						hp--;						//hp��-1
						if (hp <= 0) {
							isDead = true;		//hp��0�ɂȂ��������
							PlaySoundMem(BDestructSoundH, DX_PLAYTYPE_BACK);
							PointPlus();			//�X�R�A+10pts
						}
					}
				}
			}
		}
	}

	void CollisionPlayer() {			//������̃v���C���[�Փ˔���
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

const int BNUM = BLOCK_NUM;		//�u���b�N�̑���
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