#pragma once

#ifndef DEF_BLOCK_H //二重include防止

#define DEF_BLOCK_H

#define BLOCK_NUM 52;


// 他クラスが使う用
int GetBlockX(int n);
int GetBlockY(int n);
bool GetBlockIsDead(int n);
void ReviveBlock(int n);


// 初期化
void Block_Ini();

// 計算
void Block_Calc();

// 描画
void Block_Show();


#endif