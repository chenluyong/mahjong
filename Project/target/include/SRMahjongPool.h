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
	virtual int getMahjongPoolData(unsigned char* out_data, unsigned char size);

	// 查找当前麻将牌所在位置
	// 返回位置下标
	virtual unsigned char find(unsigned char card_data)const;

	// 检查某个麻将牌拥有的数量
	// 返回值：麻将牌的数量
	virtual unsigned char have(unsigned char card_data)const;



protected:

	unsigned char cardData_[MAX_REPERTORY + 1];

	unsigned char indexBegin_;
	unsigned char indexEnd_;


};


// 牌墙上的数据
class SRMJROBOT_API SRInvisibleMahjongPool : public SRMahjongPool {

public:
	// 前端弹出一个麻将牌
	unsigned char pop_front(void);
	// 后端弹出一个麻将牌
	unsigned char pop_back(void);
	// 设置池中数据
	int setMahjongPoolData(const unsigned char* _data, unsigned char size);

};


// 牌池中的数据
class SRMJROBOT_API SRVisibleMahjongPool : public SRMahjongPool {
public:
	// push_back
	void push_back(unsigned char data);


};

#endif // _SHANREN_MAHJONG_POOL_H_