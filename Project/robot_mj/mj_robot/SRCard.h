#ifndef _SHANREN_CARD_H_
#define _SHANREN_CARD_H_
#include "mj_header.h"
#include <vector>

class SRCard
{
private:
	//类型子项
	struct stKindItem {
		BYTE							cbWeaveKind;						//组合类型
		BYTE							cbCenterCard;						//中心扑克
		BYTE							cbCardIndex[3];						//扑克索引
	};
	struct stWinItem {
		BYTE dual;
		BYTE itemSize;
		stKindItem item[4];
		BYTE wantCard[5];
	};
public:
	SRCard();
	SRCard(const BYTE _cardIndex[]);
	virtual ~SRCard();

				   
	BYTE getOutCard(void);

	int getFanPaiOne(int _indexBegin = 0);

	// 获得间隔1个空位的不连续单牌，从两头向中间排查
	int getIntervalOne(int _indexBegin = 0);

	// 获得间隔2个空位的不连续单牌，从两头向中间排查
	int getIntervalTwo(int _indexBegin = 0);

	


	// 是否胡了
	int isWin(void);

	// 差什么牌
	std::vector<BYTE> getDeficiencyCardData(void);
	

	// 几个刻顺
	int getSumKeShun(void);


public:
	
	int operator [] (const int &index) { return cardIndex_[index]; }

private:

	int analyseCard(BYTE _cardIndex[MAX_INDEX]);


	// 检测同花色
	bool isSameColor(BYTE arg1, BYTE arg2, BYTE arg3 = 0);

	// 获得牌型权重
	int getCardWeight(BYTE card);

	// 获得刻顺数量
	int getKeAndShun(const BYTE _cardIndex[], stWinItem & _winItem);
	int getKeAndShun(const BYTE _cardIndex[]);

	int isWin(const BYTE _cardIndex[], int _duiIndex);

	int isAllKeAndShun(const BYTE _cardIndex[]);

private:
	BYTE cardIndex_[MAX_INDEX];

	stWinItem winModel_;
};

#endif // _SHANREN_CARD_H_