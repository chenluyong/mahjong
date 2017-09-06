#pragma once
#include "SRMahjongGlobal.h"


class SRMAHJONGROBOT_API SRMahjongPool
{
public:
	SRMahjongPool();
	virtual ~SRMahjongPool();

	BYTE pop_front(void) { return cardData_[iteratorBegin_++]; }
	BYTE pop_back(void) { return cardData_[iteratorEnd_--]; }

	// 获得麻将数据
	const BYTE* getCardData(void) const { return &(cardData_[iteratorBegin_]); }

	// 获得当前麻将数量
	BYTE getCardDataCount(void) const { return (iteratorBegin_ - iteratorEnd_); }

	// 查找某个牌的位置
	int find(BYTE _card) const ;

protected:

	// 获得麻将最大数量
	BYTE getCardMaxCount(void) const { return (sizeof(cardData_) / sizeof(cardData_[0])); }

protected:

	// 扑克数据
	BYTE cardData_[MAX_REPERTORY];

	BYTE iteratorBegin_;
	BYTE iteratorEnd_;

private:
	// 
};



// 牌墙上的数据
class SRMAHJONGROBOT_API SRInvisibleMahjongPool : public SRMahjongPool {

public:

	SRInvisibleMahjongPool(void);

public:
	
	// 洗牌
	int randCardData(void);


protected:

	// 洗牌
	// _cardData: 要洗的牌
	// _maxCount: 牌的数量
	int randCardData(BYTE *_cardData, BYTE _maxCount);


private:



};


// 牌池中的数据
class SRMAHJONGROBOT_API SRVisibleMahjongPool : public SRMahjongPool {

public:

	SRVisibleMahjongPool(void);

public:

	// push_back
	void push_back(BYTE data) { cardData_[iteratorEnd_++] = data; }

	


};