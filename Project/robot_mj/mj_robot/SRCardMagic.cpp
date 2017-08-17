#include "SRCardMagic.h"
#include <vector>


SRCardMagic::SRCardMagic(const stCardData& _cardData)
	:srcCardData_(_cardData) {
	// 转换成索引牌型
	memset(cardIndex_, 0, sizeof(cardIndex_));

	// 如果数据有误，可能会造成数组越界
	for (int i = 0; i < MAX_COUNT; ++i) 
		cardIndex_[switchToCardIndex(_cardData.card[i])]++;
	
	analyseCard();
}

SRCardMagic::SRCardMagic(void) {
}


SRCardMagic::~SRCardMagic() {
}

bool SRCardMagic::isValidCard(BYTE cbCardData) const {
	BYTE cbValue = (cbCardData&MASK_VALUE);
	BYTE cbColor = (cbCardData&MASK_COLOR) >> 4;
	return (((cbValue >= 1) && (cbValue <= 9) && (cbColor <= 2)) || ((cbValue >= 1) && (cbValue <= 7) && (cbColor == 3)));
	return false;
}

int SRCardMagic::getCardSize(void) const {
	BYTE cbCardCount = 0;
	for (BYTE i = 0; i<MAX_INDEX; i++)
		cbCardCount += cardIndex_[i];
	return cbCardCount;
}

int SRCardMagic::analyseCard(void)
{
	// 计算麻将数目
	BYTE cbCardCount = getCardSize();
	if ((cbCardCount<2) || (cbCardCount>MAX_COUNT) || ((cbCardCount - 2) % 3 != 0))
		return -1;
	BYTE cbCardIndex[MAX_INDEX];
	memcpy(cbCardIndex, cardIndex_, sizeof(cardIndex_));

	// 组合种类变量声明
	
	BYTE cbKindItemCount = 0;
	stKindItem KindItem[MAX_WEAVE];
	memset(KindItem, 0, sizeof(KindItem));
								 
	// 拆分分析
	if (cbCardCount >= 3)
	{
		for (BYTE i = 0; i < MAX_INDEX; i++)
		{
			//同牌判断
			if (cbCardIndex[i] >= 3) {

				if (cbCardIndex[i] == 4)
					KindItem[cbKindItemCount].cbWeaveKind = CARDTYPE_KE;
				else
					KindItem[cbKindItemCount].cbWeaveKind = CARDTYPE_GANG;

				memset(KindItem[cbKindItemCount].cbCardIndex, i, sizeof(KindItem[cbKindItemCount].cbCardIndex)); 
				KindItem[cbKindItemCount++].cbCenterCard = switchToCardData(i);

				// 无论是刻还是杠，这里的牌都不继续分析，因为已成胡牌所需
				//cbCardIndex[i] = 0;
				continue;
			}

			// 连牌判断
			if ((i<(MAX_INDEX - 9)) && (cbCardIndex[i]>0) && ((i % 9)<7))
			{
				for (BYTE j = 1; j <= cbCardIndex[i]; j++)
				{
					// 顺子的牌
					if ((cbCardIndex[i + 1] >= j) && (cbCardIndex[i + 2] >= j))
					{
						KindItem[cbKindItemCount].cbCardIndex[0] = i;
						KindItem[cbKindItemCount].cbCardIndex[1] = i + 1;
						KindItem[cbKindItemCount].cbCardIndex[2] = i + 2;
						KindItem[cbKindItemCount].cbWeaveKind = CARDTYPE_SHUN;
						KindItem[cbKindItemCount++].cbCenterCard = switchToCardData(i);
					}
				}
			}
		}
	}


	return 0;
}

BYTE SRCardMagic::switchToCardIndex(BYTE cbCardData) const {
	return ((cbCardData&MASK_COLOR) >> 4) * 9 + (cbCardData&MASK_VALUE) - 1;
}

BYTE SRCardMagic::switchToCardData(BYTE cbCardIndex) const {
	return ((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1);
}
