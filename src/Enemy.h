#pragma once

#pragma once

#ifndef DEF_ENEMY_H //二重include防止

#define DEF_ENEMY_H

#define ENEMY_SPEED 2;
#define EBULLET_NUM 40;
#define EBULLET_SPEED 6;

struct EnemyBullet {
	bool isDead;		//弾を描画するかどうか
	int bx, by;					//弾のx, y座標
	int BulImgH;		//グラフィックハンドル
	int width, height;		//画像の幅と高さ
	int vx, vy;					//弾のx, y方向速度
};


// 初期化
void Enemy_Ini();


// 計算
void Enemy_Calc();

// 描画
void Enemy_Show();


#endif