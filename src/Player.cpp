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


int Key[256]; // �L�[��������Ă���t���[�������i�[����

class Player {
private:
	int PlayerImgH;		//�v���C���[�摜�n���h��
	int BulletImgH;		//�e�̉摜�n���h��
	int x, y;						//�v���C���[��x, y���W
	double angle;			//�v���C���[�̊p�x(�`��p)
	int dir;						//�v���C���[�̌���(��������p)
	bool isDead;				//�v���C���[�̐����t���O
	bool isNoDamage;	//��e���̖��G�t���O
	int noDamageCount;  //���G���ԃJ�E���g
	int hp;						//�v���C���[HP
	bool isMoving[4];				//�ړ��\�t���O(0: ��C1: �E�C2: ���C3: ��)
	int PBulletSoundH;		//�v���C���[�̒e���ˉ��n���h��
	int PDamageSoundH;		//�v���C���[�̃_���[�W���n���h��
	int PExplosionSoundH;		//�v���C���[�̎��S���n���h��
	

	static const int BulletNum = BULLET_NUM;		//�e�̍ő吔
	Bullet bullet[BulletNum];		//�e�\���̂̔z��
	int BulletCounter;
	int TimeCounter;

public:
	Player() {
		PlayerImgH = LoadGraph("img/player.png");				//�v���C���[�摜�����[�h
		BulletImgH = LoadGraph("img/pBullet.png");			//�e�摜�����[�h
		PBulletSoundH = LoadSoundMem("sound/player_shot.mp3");		//�e���ˉ������[�h
		PDamageSoundH = LoadSoundMem("sound/player_damage.mp3");		//�_���[�W�������[�h
		PExplosionSoundH = LoadSoundMem("sound/player_explosion.mp3");		//�_���[�W�������[�h
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

	/*---------------------------�v���C���[�̊֐�----------------------------*/
	
	void Show() {		//�v���C���[�`��֐�
		if ( !isDead ) {
			//DrawRotaGraph(x, y, 0.6, angle, PlayerImgH, TRUE);		//�v���C���[�̕`��
			
			if (!isNoDamage) {
				DrawRotaGraph(x, y, 0.6, angle, PlayerImgH, TRUE);		//�v���C���[�̕`��
			}
			else {
				
				if (noDamageCount % 10 < 5)		DrawRotaGraph(x, y, 0.6, angle, PlayerImgH, TRUE);		//5�t���[�����Ƀv���C���[�̕`��
				noDamageCount++;			//���G�J�E���^���Z
				
				if (noDamageCount > 60) {	//���G���Ԃ�60�t���[����������
					isNoDamage = false;
					noDamageCount = 0;
				}
			}
			
		}
	}

	void Move() {			//�v���C���[�ړ��֐�
		//�v���C���[�ړ�
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

		//�����ύX
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
		PlaySoundMem(PDamageSoundH, DX_PLAYTYPE_BACK);		//�_���[�W�����Ȃ炷
	}
	void CalcHP() {
		if (hp <= 0) {
			isDead = true;
			//�v���C���[����ʊO��
			x = -40;
			y = -40;
			PlaySoundMem(PExplosionSoundH, DX_PLAYTYPE_BACK);		//���S�����Ƃ��̔��������Ȃ炷
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


	/*---------------------------�e�̊֐�---------------------------------------*/
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
			bullet[BulletCounter].dir = dir;					//�e�̌������v���C���[�̌����ɍ��킹��
			bullet[BulletCounter].isDead = false;			//�e������
			switch (bullet[BulletCounter].dir) {
			case 0:		//�����
				bullet[BulletCounter].bx = x;
				bullet[BulletCounter].by = y - 30;
				break;
			case 1:			//�E����
				bullet[BulletCounter].bx = x + 30;
				bullet[BulletCounter].by = y;
				break;
			case 2:		//������
				bullet[BulletCounter].bx = x;
				bullet[BulletCounter].by = y + 30;
				break;
			case 3:		//������
				bullet[BulletCounter].bx = x - 30;
				bullet[BulletCounter].by = y;
				break;
			default:
				bullet[BulletCounter].isDead = true;
			}
			PlaySoundMem(PBulletSoundH, DX_PLAYTYPE_BACK);
			BulletCounter = (BulletCounter+1) % 30;		//���̒e�𑕓U
		}
	}

	void BulletDraw() {
		for (int i = 0; i < BulletNum; i++) {
			if (!bullet[i].isDead) {
				DrawRotaGraph(bullet[i].bx, bullet[i].by, 0.2, 0.0, bullet[i].BulImgH, TRUE);		//�e��`��
				switch (bullet[i].dir) {
				case 0:		//�����
					bullet[i].by -= BULLET_SPEED;
					break;
				case 1:			//�E����
					bullet[i].bx += BULLET_SPEED;
					break;
				case 2:		//������
					bullet[i].by += BULLET_SPEED;
					break;
				case 3:		//������
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

// �L�[�̓��͏�Ԃ��X�V����
int UpdateKey() {
	char tmpKey[256]; // ���݂̃L�[�̓��͏�Ԃ��i�[����
	GetHitKeyStateAll(tmpKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�
	for (int i = 0; i<256; i++) {
		if (tmpKey[i] != 0) { // i�Ԃ̃L�[�R�[�h�ɑΉ�����L�[��������Ă�����
			Key[i]++;     // ���Z
		}
		else {              // ������Ă��Ȃ����
			Key[i] = 0;   // 0�ɂ���
		}
	}
	return 0;
}


Player* p;						//�v���C���["p"��錾

int GetBulletX(int n) {		//���̃N���X�ɒe��x���W��n��
	return p->GetBullet(n).bx;
}
int GetBulletY(int n) {		//���̃N���X�ɒe��y���W��n��
	return p->GetBullet(n).by;
}
bool GetBulletIsDead(int n) {		//���̃N���X�ɒe�̐�����Ԃ�
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

void Player_Ini() {		//�v���C���[�������֐�
	p = new Player();					//�v���C���[�̃C���X�^���X�𐶐�
	p->IniBullet();						//	�e�̏�����
}

void Player_Calc() {		//�v���C���[�Ɋւ��鉉�Z
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

void Player_Show() {		//�v���C���[�`��֐�
	p->Show();
	p->BulletDraw();			//�v���C���[�̒e�̕`��

	/*
	ChangeFont("�l�r ����");
	DrawFormatString(0, 0, GetColor(0, 255, 0), "�v���C���[���W[%d,%d]", p->GetPX(), p->GetPY()); // �v���C���[���W��`�悷��@��(�f�o�b�O�p)
	*/
}