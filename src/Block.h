#pragma once

#ifndef DEF_BLOCK_H //��dinclude�h�~

#define DEF_BLOCK_H

#define BLOCK_NUM 52;


// ���N���X���g���p
int GetBlockX(int n);
int GetBlockY(int n);
bool GetBlockIsDead(int n);
void ReviveBlock(int n);


// ������
void Block_Ini();

// �v�Z
void Block_Calc();

// �`��
void Block_Show();


#endif