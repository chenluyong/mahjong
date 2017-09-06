#include "SRMahjongPool.h"

#include <time.h>
#include <iostream>
#include <vector>



SRMahjongPool::SRMahjongPool() {
	// 初始化牌数据
	memset(cardData_, 0, sizeof(cardData_));
	iteratorBegin_ = 0;
	iteratorEnd_ = getCardMaxCount();
}


SRMahjongPool::~SRMahjongPool()
{
	
}

int SRMahjongPool::find(BYTE _card) const {
	auto iter_begin = iteratorBegin_;

	while (cardData_[iter_begin++] != _card);

	if (iter_begin > iteratorEnd_)
		return -1;
	else
		return (iter_begin - iteratorBegin_);
}



////////////////////////////SRInvisibleMahjongPool////////////////////////////



SRInvisibleMahjongPool::SRInvisibleMahjongPool(void)
{
}

int SRInvisibleMahjongPool::randCardData(void) {
	memset(cardData_, 0, sizeof(cardData_));
	return randCardData(cardData_, (sizeof(cardData_) / sizeof(cardData_[0])));
}

int SRInvisibleMahjongPool::randCardData(BYTE * _cardData, BYTE _maxCount) {
	if (_cardData == nullptr)
		return -87;


	//混乱准备
	BYTE cbCardDataTemp[MAX_REPERTORY] = {};
	memcpy(cbCardDataTemp, g_cbCardDataArray, sizeof(g_cbCardDataArray));

	//初始化种子
	srand((unsigned int)time(0));

	//混乱扑克
	BYTE cbRandCount = 0, cbPosition = 0;
	do {
		cbPosition = rand() % (_maxCount - cbRandCount);
		_cardData[cbRandCount++] = cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition] = cbCardDataTemp[_maxCount - cbRandCount];
	} while (cbRandCount<_maxCount);

	return 0;
}

SRVisibleMahjongPool::SRVisibleMahjongPool(void)
{
	iteratorEnd_ = 0;
}
