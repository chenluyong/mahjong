#ifndef _SHANREN_MAHJONG_POOL_H_
#define _SHANREN_MAHJONG_POOL_H_

#include "srmjglobal.h"

#ifdef SRMJROBOT_EXPORTS
#define SRMJROBOT_API __declspec(dllexport)
#else
#define SRMJROBOT_API __declspec(dllimport)
#endif



class SRMJROBOT_API SRMahjongPool
{
public:

	virtual ~SRMahjongPool(void);


public:

	// 获得池中数据
	int getMahjongPoolData(unsigned char* out_data, unsigned char size);

	// 查找当前麻将牌所在位置
	// 返回位置下标
	unsigned char find(unsigned char card_data)const;

	// 检查某个麻将牌拥有的数量
	// 返回值：麻将牌的数量
	unsigned char have(unsigned char card_data)const;

	// 前端弹出一个麻将牌
	unsigned char pop_front(void);
	// 后端弹出一个麻将牌
	unsigned char pop_back(void);


private:

	unsigned char cardData_[MAX_COUNT + 1];

	unsigned char indexBegin_;
	unsigned char indexEnd_;


};

#endif // _SHANREN_MAHJONG_POOL_H_