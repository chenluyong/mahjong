#ifndef _SHANREN_CARD_H_
#define _SHANREN_CARD_H_
#include "mj_header.h"
#include <vector>

class SRCard
{
public:
	SRCard();
	SRCard(const BYTE _cardIndex[]);
	virtual ~SRCard();



	// 去除间隔1个空位的不连续单牌，从两头向中间排查
	int getIntervalOne(int _indexBegin = 0);

	// 去除间隔2个空位的不连续单牌，从两头向中间排查
	int getIntervalTwo(int _indexBegin = 0);

	


	// 是否胡了
	int isWin(void);

	// 差什么牌
	std::vector<BYTE> getDeficiencyCardData(void);
	

	// 几个刻顺
	int getSumKeShun(void);


private:
	bool isSameColor(BYTE arg1, BYTE arg2, BYTE arg3 = 0);

	int getKeAndShun(const BYTE _cardIndex[]);

	int isWin(const BYTE _cardIndex[], int _duiIndex);

private:
	BYTE cardIndex_[MAX_INDEX];
};

#endif // _SHANREN_CARD_H_