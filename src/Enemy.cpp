#include "DxLib.h"
#include "Block.h"
#include "Enemy.h"
#include "Player.h"
#include <math.h>

const int BNUM = BLOCK_NUM;		//�u���b�N�̑���
const int EBUL_NUM = EBULLET_NUM;
const int EBUL_SPEED = EBULLET_SPEED;
const int P_SIZE = PLAYER_SIZE;
const int E_SIZE = 40;

class Enemy {
private:
	int x, y;
	int speed;
	double angle;				//��]���Ă�p�x
	int dang;						//���v���Ȃ�1, �����v���Ȃ�-1
	int moveCount;
	int EnemyImgH;			//�G�摜�n���h��
	int EBulletImgH;			//�G�̒e�摜�n���h��
	int bn;							//���݂���u���b�N�̔z��Y��
	int nextbn;					//���̈ړ���u���b�N�̔z��Y��
	bool isMoving;
	bool isClockwise;		//���v��肩�����łȂ���
	int EBulletSoundH;	//�G�̎ˌ����n���h��
	int ECreateSoundH;	//�G�̃u���b�N�������n���h��
	
	EnemyBullet eBullet[EBUL_NUM];
	int bulletCounter;

public:
	Enemy(int n, bool iniClockwise) {
		speed = ENEMY_SPEED;
		EnemyImgH = LoadGraph("img/enemy1.png");
		EBulletImgH = LoadGraph("img/eBullet.png");
		EBulletSoundH = LoadSoundMem("sound/enemy_shot.mp3");		//�ˌ��������[�h
		ECreateSoundH = LoadSoundMem("sound/enemy_create.mp3");		//�u���b�N�����������[�h
		bn = n;
		nextbn = n;
		angle = 0;
		dang = 1;
		moveCount = 0;
		x = GetBlockX(n);
		y = GetBlockY(n);
		isClockwise = iniClockwise;
		isMoving = !GetBlockIsDead(n);
		IniBullet();		//�e������
		bulletCounter = 0;
	}

	/*---------------- �G�{�̂̊֐� -----------------------*/
	void DecisionBehavior() {		//�ړ����������s�������߂�
		if (moveCount == 0) {
			bn = nextbn;
			x = GetBlockX(bn);
			y = GetBlockY(bn);
			if (GetBlockIsDead(bn)) {
				isMoving = false;
			}
			else {
				if (isClockwise) {				//���v���Ȃ�
					nextbn = (bn + 1) % BNUM;			//�z��𐳂̕����Ƀ��[�v�ړ�
					dang = 1;
				}
				else {									//�����v���Ȃ�
					nextbn = (bn + (BNUM - 1)) % BNUM;		//�z��𕉂̕����Ƀ��[�v�ړ�
					dang = -1;
				}
				isMoving = true;
			}
			moveCount++;
		}
	}

	void Move() {		//�G�ړ��֐�
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
				ReviveBlock(bn);		//�u���b�N�𐶐�����
				PlaySoundMem(ECreateSoundH, DX_PLAYTYPE_BACK);		//�u���b�N���������Ȃ炷
				moveCount = 0;
			}
		}
	}

	//�v���C���[�Ƃ̏Փ˔���(����)
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
	/*---------------- �ȏ�G�{�̂̊֐� -----------------------*/

	/*---------------- �G�̒e�p�֐� -----------------------*/
	void IniBullet() {		//�e�������֐�
		for (int i = 0; i < EBUL_NUM; i++) {
			eBullet[i].isDead = true;
			eBullet[i].vx = 0;
			eBullet[i].vy = 0;
		}
	}

	void Shot() {		//�e���ˊ֐�
		if (moveCount % 4 == 0 && isMoving) {
			eBullet[bulletCounter].isDead = false;
			eBullet[bulletCounter].vx = (int)(cos(angle) * EBUL_SPEED);
			eBullet[bulletCounter].vy = (int)(sin(angle) * EBUL_SPEED);
			eBullet[bulletCounter].bx = x + eBullet[bulletCounter].vx;
			eBullet[bulletCounter].by = y + eBullet[bulletCounter].vy;

			PlaySoundMem(EBulletSoundH, DX_PLAYTYPE_BACK);		//�ˌ������Ȃ炷

			bulletCounter = (bulletCounter + 1) % EBUL_NUM;		//���̒e�𑕓U
		}
	}

	void BulletDraw() {		//�e�̕`��A�����蔻��
		for (int i = 0; i < EBUL_NUM; i++) {
			if (!eBullet[i].isDead) {
				DrawRotaGraph(eBullet[i].bx, eBullet[i].by, 0.2, 0.0, EBulletImgH, TRUE);		//�e��`��
				eBullet[i].bx += eBullet[i].vx;
				eBullet[i].by += eBullet[i].vy;

				if (eBullet[i].by < -10 || eBullet[i].by > 480 || eBullet[i].bx < -10 || eBullet[i].bx > 650) {
					eBullet[i].isDead = true;
				}

				//�v���C���[�Ƃ̓����蔻��
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

	/*---------------- �ȏ�G�̒e�p�֐� ------------------*/
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