#ifndef _SHANREN_CARD_MAGIC_H_
#define _SHANREN_CARD_MAGIC_H_

#include "mj_header.h"


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


	int getCardSize(void) const;

private:

	//分析扑克
	int analyseCard(void);



private:

	//有效判断
	bool isValidCard(BYTE cbCardData) const;

	//扑克转换
	BYTE switchToCardIndex(BYTE cbCardData) const;

	//扑克转换
	BYTE switchToCardData(BYTE cbCardIndex) const;


private:

	BYTE cardIndex_[MAX_INDEX];
	stCardData srcCardData_;
};

#endif // _SHANREN_CARD_MAGIC_H_