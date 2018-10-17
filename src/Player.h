#pragma once

#ifndef DEF_PLAYER_H //二重include防止

#define DEF_PLAYER_H

#define PLAYER_SPEED 5;
#define BULLET_SPEED 8;
#define BULLET_NUM 30;
#define PLAYER_SIZE 28;

struct Bullet {
		bool isDead;		//弾を描画するかどうか
		int bx, by;					//弾のx, y座標
		int BulImgH;		//グラフィックハンドル
		int width, height;		//画像の幅と高さ
		int dir;						//弾の向き
};



//他クラスが使う用
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

// 初期化
void Player_Ini();

// 計算
void Player_Calc();

// 描画
void Player_Show();

#endif 