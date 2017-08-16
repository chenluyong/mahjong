#ifndef SHANREN_CMD_SPARROW_H
#define SHANREN_CMD_SPARROW_H

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

//////////////////////////////////////////////////////////////////////////
// 公共宏定义

#define GAME_PLAYER					4									//游戏人数


// 常量定义
#define MAX_WEAVE					5									//最大组合
#define MAX_INDEX					34									//最大索引
#define MAX_COUNT					17									//最大数目
#define MAX_REPERTORY				136									//最大库存

enum enDirection {
	South = 0,              //南向 下
	West,					//西向 左
	North,                  //北向 上
	East,					//东向 右
};


		
#endif // SHANREN_CMD_SPARROW_H