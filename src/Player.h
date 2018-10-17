#pragma once

#ifndef DEF_PLAYER_H //��dinclude�h�~

#define DEF_PLAYER_H

#define PLAYER_SPEED 5;
#define BULLET_SPEED 8;
#define BULLET_NUM 30;
#define PLAYER_SIZE 28;

struct Bullet {
		bool isDead;		//�e��`�悷�邩�ǂ���
		int bx, by;					//�e��x, y���W
		int BulImgH;		//�O���t�B�b�N�n���h��
		int width, height;		//�摜�̕��ƍ���
		int dir;						//�e�̌���
};



//���N���X���g���p
int GetBulletX(int n);
int GetBulletY(int n);
bool GetBulletIsDead(int n);
void KillBullet(int n);
bool GetPlayerIsNoDamage();
void DamagePlayer();
int GetPlayerX();
int GetPlayerY();
bool GetPlayerIsMoving(int n);
void CollisionP_Up();
void CollisionP_Right();
void CollisionP_Down();
void CollisionP_Left();
void KillPlayer();
int GetPlayerHP();

// ������
void Player_Ini();

// �v�Z
void Player_Calc();

// �`��
void Player_Show();

#endif 