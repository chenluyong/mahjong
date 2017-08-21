#ifndef _SHANREN_CARD_MAGIC_H_
#define _SHANREN_CARD_MAGIC_H_

#include "mj_header.h"
#include <vector>


class SRCardMagic
{

	//类型子项
	struct stKindItem
	{
		BYTE							cbWeaveKind;						//组合类型
		BYTE							cbCenterCard;						//中心扑克
		BYTE							cbCardIndex[3];						//扑克索引
	};

public:
	SRCardMagic(const stCardData& _cardData);
	SRCardMagic(void);

	virtual ~SRCardMagic();


	// 获得卡牌大小
	int getCardSize(void) const;
	// 获得牌型数据
	int getCardIndexData(BYTE* _out_cardIndex, BYTE* _out_count = nullptr);
	// 添加一张牌
	int addCardData(BYTE _card);
	// 删除一张牌
	int delCardData(BYTE _card);

	// 获得需求的牌型
	std::vector<BYTE> getRequire(void) const;


public:

	static int isTing(const BYTE _cardIndex[], int _indexBegin = 0);

	// 判定胡牌
	static int isWin(const BYTE _cardIndex[], int _duiIndex = 0);

	static int isSameColor(BYTE _arg1, BYTE _arg2, BYTE _arg3 = 0);

	// 返回下标
	static int isIntervalTwo(const BYTE _cardIndex[]);
	// 返回下标
	static int isIntervalOne(const BYTE _cardIndex[]);

	// 获得当前牌的刻顺数量
	static int getKeAndShun(const BYTE _cardIndex[]);

	//有效判断
	static bool isValidCard(BYTE cbCardData);

	//扑克转换
	static BYTE switchToCardIndex(BYTE cbCardData);

	//扑克转换
	static BYTE switchToCardData(BYTE cbCardIndex);
private:

	//分析扑克
	int analyseCard(void);



private:



private:

	BYTE cardIndex_[MAX_INDEX];
	stCardData srcCardData_;

};

#endif // _SHANREN_CARD_MAGIC_H_