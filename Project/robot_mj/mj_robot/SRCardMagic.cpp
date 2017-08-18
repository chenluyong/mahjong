#include "SRCardMagic.h"
#include <vector>


SRCardMagic::SRCardMagic(const stCardData& _cardData)
	:srcCardData_(_cardData) {
	// 转换成索引牌型
	memset(cardIndex_, 0, sizeof(cardIndex_));

	// 数据转换为索引模式(如果数据有误，可能会造成数组越界)
	for (int i = 0; i < MAX_COUNT; ++i) 
		cardIndex_[switchToCardIndex(_cardData.card[i])]++;
	
	int i = isTing(cardIndex_);
	analyseCard();
}

SRCardMagic::SRCardMagic(void) {
}


SRCardMagic::~SRCardMagic() {
}

bool SRCardMagic::isValidCard(BYTE cbCardData) {
	BYTE cbValue = (cbCardData&MASK_VALUE);
	BYTE cbColor = (cbCardData&MASK_COLOR) >> 4;
	return (((cbValue >= 1) && (cbValue <= 9) && (cbColor <= 2)) || ((cbValue >= 1) && (cbValue <= 7) && (cbColor == 3)));
}

int SRCardMagic::getCardSize(void) const {
	BYTE cbCardCount = 0;
	for (BYTE i = 0; i<MAX_INDEX; i++)
		cbCardCount += cardIndex_[i];
	return cbCardCount;
}

int SRCardMagic::getCardIndexData(BYTE * _out_cardIndex, BYTE * _out_count)
{
	if (!_out_cardIndex)
		return -1;

	memcpy(_out_cardIndex, cardIndex_, MAX_INDEX);

	if (_out_count)
		*_out_count = (BYTE)MAX_INDEX;

	return 0;
}

int SRCardMagic::addCardData(BYTE _card)
{
	return 0;
}

int SRCardMagic::delCardData(BYTE _card)
{
	return 0;
}

		   
int SRCardMagic::isTing(const BYTE _cardIndex[])
{
	BYTE card_index[MAX_INDEX] = {};
	memcpy(card_index, _cardIndex, sizeof(_cardIndex[0]) * MAX_INDEX);

	for (int i = 0; i < MAX_INDEX; ++i) {
		++card_index[i];

		if (0 == isWin(card_index))
			return 0;

		--card_index[i];
	}
	return -1;
}

int SRCardMagic::isWin(const BYTE _cardIndex[], int _duiIndex)
{
	BYTE card_index[MAX_INDEX] = {};
	memcpy(card_index, _cardIndex, sizeof(_cardIndex[0])*MAX_INDEX);

	
	for (int i = 0, k = 0; i < MAX_INDEX; ++i) {
		if (card_index[i] >= 2) {
			// 该将是否已判定过
			if (++k < _duiIndex)
				continue;

			// 把将牌取出
			card_index[i] -= 2;

			// 开始判定刻顺数量
			if (MAX_WEAVE == getKeAndShun(card_index))
				return 0;
			else
				return isWin(_cardIndex, ++_duiIndex);
		}
	}


	
	return -1;
}

int SRCardMagic::getKeAndShun(const BYTE _cardIndex[]) {
	// 不修改原值
	BYTE cbCardIndex[MAX_INDEX] = {};
	memcpy(cbCardIndex, _cardIndex, sizeof(_cardIndex[0])* MAX_INDEX);

	// 组合数量变量声明		
	BYTE cbKindItemCount = 0;

	// 拆分分析
	for (BYTE i = 0; i < MAX_INDEX; i++) {
		//同牌判断
		if (cbCardIndex[i] >= 3) {
			++cbKindItemCount;
			cbCardIndex[i] -= 3;
		}
		// 连牌判断
		else if ((i<(MAX_INDEX - 9)) && (cbCardIndex[i]>0) && ((i % 9)<7)) {
			for (;1 <= cbCardIndex[i];) {
				// 顺子的牌
				if ((cbCardIndex[i + 1] >= 1) && (cbCardIndex[i + 2] >= 1)) {
					++cbKindItemCount;
					cbCardIndex[i] -= 1;
					cbCardIndex[i + 1] -= 1;
					cbCardIndex[i + 2] -= 1;
				}
				else
					break;
			}
				
		}
	}	 
	return cbKindItemCount;
}

int SRCardMagic::analyseCard(void)
{
	//// 计算麻将数目
	//BYTE cbCardCount = getCardSize();
	//if ((cbCardCount<2) || (cbCardCount>MAX_COUNT) || ((cbCardCount - 2) % 3 != 0))
	//	return -1;
	//BYTE cbCardIndex[MAX_INDEX];
	//memcpy(cbCardIndex, cardIndex_, sizeof(cardIndex_));

	//// 组合种类变量声明
	//
	//BYTE cbKindItemCount = 0;
	//stKindItem KindItem[MAX_WEAVE];
	//memset(KindItem, 0, sizeof(KindItem));
	//							 
	//// 拆分分析
	//if (cbCardCount >= 3)
	//{
	//	for (BYTE i = 0; i < MAX_INDEX; i++)
	//	{
	//		bool haveKind = false;
	//		//同牌判断
	//		if (cbCardIndex[i] >= 3) {

	//			if (cbCardIndex[i] == 4)
	//				KindItem[cbKindItemCount].cbWeaveKind = CARDTYPE_KE;
	//			else
	//				KindItem[cbKindItemCount].cbWeaveKind = CARDTYPE_GANG;

	//			memset(KindItem[cbKindItemCount].cbCardIndex, i, sizeof(KindItem[cbKindItemCount].cbCardIndex)); 
	//			KindItem[cbKindItemCount++].cbCenterCard = switchToCardData(i);

	//			// 无论是刻还是杠，这里的牌都不继续分析，因为已成胡牌所需
	//			//cbCardIndex[i] = 0;
	//			//continue;
	//			haveKind = true;
	//		}

	//		// 连牌判断
	//		else if ((i<(MAX_INDEX - 9)) && (cbCardIndex[i]>0) && ((i % 9)<7))
	//		{
	//			for (BYTE j = 1; j <= cbCardIndex[i]; j++)
	//			{
	//				// 顺子的牌
	//				if ((cbCardIndex[i + 1] >= j) && (cbCardIndex[i + 2] >= j))
	//				{
	//					KindItem[cbKindItemCount].cbCardIndex[0] = i;
	//					KindItem[cbKindItemCount].cbCardIndex[1] = i + 1;
	//					KindItem[cbKindItemCount].cbCardIndex[2] = i + 2;
	//					KindItem[cbKindItemCount].cbWeaveKind = CARDTYPE_SHUN;
	//					KindItem[cbKindItemCount++].cbCenterCard = switchToCardData(i);

	//					haveKind = true;
	//				}
	//			}
	//		}

	//		// 孤张判断
	//		if (!haveKind) {

	//		}

	//	}
	//}


	return 0;
}

BYTE SRCardMagic::switchToCardIndex(BYTE cbCardData) {
	return ((cbCardData&MASK_COLOR) >> 4) * 9 + (cbCardData&MASK_VALUE) - 1;
}

BYTE SRCardMagic::switchToCardData(BYTE cbCardIndex) {
	return ((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1);   
}
