#include "SRCard.h"



SRCard::SRCard()
{
}

SRCard::SRCard(const BYTE _cardIndex[])
{
	memcpy(cardIndex_, _cardIndex, sizeof(_cardIndex[0])*MAX_INDEX);
}


SRCard::~SRCard()
{
}

int SRCard::getIntervalOne(int _indexBegin)
{			   // 没有番牌的最大下标
	const int cMax_Index = MAX_INDEX - 7;

	BYTE card_index[MAX_INDEX] = {};
	memcpy(card_index, cardIndex_, sizeof(cardIndex_[0])*MAX_INDEX);

	for (int i = 0; i < 4; ++i) {
		const int& INDEX_END = i * 9 + 9;

		for (int index_begin = i * 9, INDEX_BEGIN = i * 9; index_begin < INDEX_END; ++index_begin) {

			// 没有牌 或者 牌在指定的范围之前 都选择跳过
			if (card_index[index_begin] == 0 || index_begin < _indexBegin)
				continue;

			const int arg1 = index_begin - 1;
			const int arg3 = index_begin + 1;

			// 判断相邻
			if ((arg1 > INDEX_BEGIN && card_index[arg1] > 0)
				|| (arg3 < INDEX_END && card_index[arg3] > 0)) {
				continue;
			}

			// 若都没有跳过，则为孤张
			return index_begin;

		}
	}

	return 0;
}

int SRCard::getIntervalTwo(int _indexBegin) {
	// 没有番牌的最大下标
	const int cMax_Index = MAX_INDEX - 7;

	BYTE card_index[MAX_INDEX] = {};
	memcpy(card_index, cardIndex_, sizeof(cardIndex_[0])*MAX_INDEX);

	for (int i = 0; i < 4; ++i) {
		const int& INDEX_END = i * 9 + 9;

		for (int index_begin = i * 9, INDEX_BEGIN = i * 9; index_begin < INDEX_END; ++index_begin) {

			// 没有牌 或者 牌在指定的范围之前 都选择跳过
			if (card_index[index_begin] == 0 || index_begin < _indexBegin)
				continue;

			const int arg1 = index_begin - 1, arg2 = index_begin - 2;
			const int arg3 = index_begin + 1, arg4 = index_begin + 2;

			// 判断相邻
			if ((arg1 > INDEX_BEGIN && card_index[arg1] > 0)
				|| (arg2 > INDEX_BEGIN && card_index[arg2] > 0)
				|| (arg3 < INDEX_END && card_index[arg3] > 0)
				|| (arg4 < INDEX_END && card_index[arg4] > 0)) {
				continue;
			}

			// 若都没有跳过，则为孤张
			return index_begin;
		}
	}

	return 0;
}

int SRCard::isWin(void)
{
	return isWin(cardIndex_, 0);
}

std::vector<BYTE> SRCard::getDeficiencyCardData(void)
{
	return std::vector<BYTE>();
}

int SRCard::getSumKeShun(void)
{
	return getKeAndShun(cardIndex_);
}


bool SRCard::isSameColor(BYTE arg1, BYTE arg2, BYTE arg3)
{
	return false;
}

int SRCard::getKeAndShun(const BYTE _cardIndex[]) {
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
			for (; 1 <= cbCardIndex[i];) {
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


int SRCard::isWin(const BYTE _cardIndex[], int _duiIndex)
{			//计算数目				 
	BYTE card_index[MAX_INDEX] = {};
	memcpy(card_index, _cardIndex, sizeof(_cardIndex[0])*MAX_INDEX);
	BYTE cbCardCount = 0;

	for (BYTE i = 0; i < MAX_INDEX; i++)
		cbCardCount += card_index[i];

	if ((cbCardCount < 2) || (cbCardCount > MAX_COUNT) || ((cbCardCount - 2) % 3 != 0))
		return -1;



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
