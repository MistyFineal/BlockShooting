#pragma once

#pragma once

#ifndef DEF_ENEMY_H //��dinclude�h�~

#define DEF_ENEMY_H

#define ENEMY_SPEED 2;
#define EBULLET_NUM 40;
#define EBULLET_SPEED 6;

struct EnemyBullet {
	bool isDead;		//�e��`�悷�邩�ǂ���
	int bx, by;					//�e��x, y���W
	int BulImgH;		//�O���t�B�b�N�n���h��
	int width, height;		//�摜�̕��ƍ���
	int vx, vy;					//�e��x, y�������x
};


// ������
void Enemy_Ini();


// �v�Z
void Enemy_Calc();

// �`��
void Enemy_Show();


#endif