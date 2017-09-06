#include "SRMahjong.h"
#include <iostream>


SRMahjong::SRMahjong(void) {
	memset(cardData_, 0, sizeof(cardData_));
	memset(cardIndex_, 0, sizeof(cardIndex_));
}


SRMahjong::~SRMahjong(void) {
}

void SRMahjong::setCardData(const BYTE * _cardData, BYTE _cardCount) {
	memset(cardData_, 0, sizeof(cardData_));
	memcpy(cardData_, _cardData, _cardCount * sizeof(_cardData[0]));
	upCardIndex();
}

int SRMahjong::removeCardData(BYTE _card) {
	BYTE* pcard = std::find(cardData_, &cardData_[MAX_COUNT], _card);

	if (pcard != nullptr)
		*pcard = 0;
	else {
		std::cout << "error: Try to delete a data that doesn't exist." << std::endl;
		return -1;
	}
	this->upCardIndex();

	return 0;
}

int SRMahjong::addCardData(BYTE _card) {

	BYTE* pcard = std::find(cardData_, &cardData_[MAX_COUNT], 0);

	if (pcard != nullptr)
		*pcard = _card;
	else {
		std::cout << "error: Try to delete a data that doesn't exist." << std::endl;
		return -1;
	}

	lastTouchCard_ = _card;

	this->upCardIndex();
	return 0;
}

int SRMahjong::count(void) const {
	auto max_size = maxSize();
	return (int)(max_size - std::count(cardData_, &cardData_[max_size - 1], 0));
}

void SRMahjong::upCardIndex(void) {
	memset(cardIndex_, 0, sizeof(cardIndex_));
	auto max_size = maxSize();
	for (int i = 0; i < max_size; ++i) {
		if (cardData_[i] == 0)
			continue;

		int idx = switchToCardIndex(cardData_[i]);

		// 防止idx下标为-1
		if (idx >= 0 && idx < MAX_INDEX)
			++cardIndex_[idx];
		else
			std::cout << "warning:" << __FUNCTION__ << std::endl;
	}
}

int SRMahjong::maxSize(void) const {
	return (sizeof(cardData_) / sizeof(cardData_[0]));
}

int SRMahjong::have(BYTE _card) {
	BYTE index = switchToCardIndex(_card);
	return cardIndex_[index];
}

//////////////////////SRMahjongCard////////////////////////


SRMahjongCard::SRMahjongCard(void) {
	memset(cardIndexState_, 0, sizeof(cardIndexState_));
}

int SRMahjongCard::getFanPaiOne(int _indexBegin) const {
	if (_indexBegin == 0)
		_indexBegin = MAX_INDEX - MAX_FAN;
	else if (_indexBegin < MAX_INDEX - MAX_FAN || _indexBegin >= MAX_INDEX)
		return -1;

	for (int i = _indexBegin; i < MAX_INDEX; ++i) {
		if (cardIndex_[i] == 1) {
			return i;
		}
	}

	return -1;
}

					  


int SRMahjongCard::getIntervalOne(int _indexBegin) const {
	for (int i = _indexBegin / 9; i < 3; ++i) {
		const int& INDEX_BEGIN = (i * 9 > _indexBegin) ? i * 9 : _indexBegin;
		const int& INDEX_END = i * 9 + 9;
		for (int index_begin = INDEX_BEGIN; index_begin < INDEX_END; ++index_begin) {
			// 没有牌 或者 牌在指定的范围之前 都选择跳过
			if (cardIndex_[index_begin] == 0)
				continue;

			const int& arg1 = index_begin - 1;
			const int& arg3 = index_begin + 1;

			// 判断相邻
			if ((arg1 >= INDEX_BEGIN && cardIndex_[arg1] > 0)
				|| (arg3 < INDEX_END && cardIndex_[arg3] > 0)) {
				continue;
			}

			// 不连续的牌
			return index_begin;
		}
	}


	return -1;
}

int SRMahjongCard::getIntervalTwo(int _indexBegin) const {

	for (int i = _indexBegin / 9; i < 3; ++i) {
		const int& INDEX_BEGIN = (i * 9 > _indexBegin) ? i * 9 : _indexBegin;
		const int& INDEX_END = i * 9 + 9;
		for (int index_begin = INDEX_BEGIN; index_begin < INDEX_END; ++index_begin) {
			// 没有牌 或者 牌在指定的范围之前 都选择跳过
			if (cardIndex_[index_begin] == 0)
				continue;

			const int& arg1 = index_begin - 1, &arg2 = index_begin - 2;
			const int& arg3 = index_begin + 1, &arg4 = index_begin + 2;

			// 判断相邻
			if ((arg1 >= INDEX_BEGIN && cardIndex_[arg1] > 0)
				|| (arg2 >= INDEX_BEGIN && cardIndex_[arg2] > 0)
				|| (arg3 < INDEX_END && cardIndex_[arg3] > 0)
				|| (arg4 < INDEX_END && cardIndex_[arg4] > 0)) {
				continue;
			}

			// 不连续的牌
			return index_begin;
		}
	}
	return -1;
}

int SRMahjongCard::getShunDopant(int _indexBegin) const {
	// 组合数量变量声明		
	BYTE kind_item_count = 0;
	stTeamKind team_kind[MAX_COUNT];
	memset(team_kind, 0, sizeof(team_kind));

	for (BYTE i = _indexBegin; i < MAX_INDEX; i++) {
		if ((i >(MAX_INDEX - 4)) || (cardIndex_[i] == 0) || ((i % 9) > 7))
			continue;

		if ((cardIndex_[i + 1] >= 1) && (cardIndex_[i + 2] >= 1))
		{
			team_kind[kind_item_count].card[0] = i;
			team_kind[kind_item_count].card[1] = i + 1;
			team_kind[kind_item_count].card[2] = i + 2;
			team_kind[kind_item_count].kind = WIK_LEFT;
			team_kind[kind_item_count++].eye = i;
		}

	}


	for (int i = 0; i < kind_item_count; ++i) {
		const int& ridx1 = team_kind[i].card[0];
		const int& ridx2 = team_kind[i].card[1];
		const int& ridx3 = team_kind[i].card[2];

		// 顺子牌的总张数
		int temp_team_count = cardIndex_[ridx1] + cardIndex_[ridx2] + cardIndex_[ridx3];

		// 说明出现 单牌 例如 [1123   1223  1122333 1112223333]
		if (temp_team_count % 3 == 1) {
			// 寻找最小的数量  
			const int& rnum = (cardIndex_[ridx1] <= cardIndex_[ridx2])
				? cardIndex_[ridx1] : cardIndex_[ridx2];


			if (cardIndex_[ridx1] != rnum) {
				return ridx1;
			}
			else if (cardIndex_[ridx2] != rnum) {
				return ridx2;
			}
			else if (cardIndex_[ridx3] != rnum) {
				// 这里需要深入细化一下  
				const int& temp_next_kind_head = team_kind[i + 1].eye;
				if (temp_next_kind_head <= ridx3 + 1) {
					// 下一个组里出现了与当前组相关的麻将 例如会出现 [123345 12335789]
					// 这里可以深入分析一下
					continue;
				}
				return ridx3;
			}
		}
	}

	return -1;
}

// [0abb0  0aab0]
// 返回牌眼 例如  [122 返回1，  223 返回2]
int SRMahjongCard::getDanDui(int _indexBegin) const {
	// 组合数量变量声明		
	BYTE kind_item_count = 0;
	stTeamKind team_kind[MAX_COUNT];
	memset(team_kind, 0, sizeof(team_kind));


	// 得到所有 附和 [0ab0] 的牌型
	for (BYTE i = _indexBegin; i < MAX_INDEX - 4; i++) {
		if (cardIndex_[i] == 0)
			continue;

		// 判断是否在一个花色中
		const BYTE& idex1 = i % 9;
		const BYTE& idex2 = (i + 1) % 9;
		if (idex1 > idex2)
			continue;

		// 判断是否附和要求  [ab0]
		if ((cardIndex_[i + 1] >= 1) && (cardIndex_[i + 2] == 0))
		{
			// 判断是否附和要求  [0ab]
			if (i % 9 != 0) {
				if (cardIndex_[i - 1] != 0)
					continue;
			}

			team_kind[kind_item_count].card[0] = i;
			team_kind[kind_item_count].card[1] = i + 1;
			team_kind[kind_item_count].kind = WIK_RIGHT;
			team_kind[kind_item_count++].eye = i;
		}

	}


	for (int i = 0; i < kind_item_count; ++i) {
		const int& ridx1 = team_kind[i].card[0];
		const int& ridx2 = team_kind[i].card[1];

		// 顺子牌的总张数
		int temp_team_count = cardIndex_[ridx1] + cardIndex_[ridx2];

		// 说明出现 单牌 例如 [112 122]
		if (temp_team_count != 3)
			continue;

		return team_kind[i].eye;
	}

	return -1;
}

int SRMahjongCard::getNotShunKe(int _indexBegin) const {
	// 不修改源数据
	unsigned char card_index[MAX_INDEX] = {};
	memcpy(card_index, cardIndex_, sizeof(cardIndex_));

	// 拆分分析
	for (int i = _indexBegin; i < MAX_INDEX - 4; i++) {
		if (card_index[i] == 0)
			continue;

		//同牌判断
		if (card_index[i] == 3) {
			continue;
		}
		// 连牌判断
		else if ((i<(MAX_INDEX - 4)) && (card_index[i]>0) && ((i % 9)<7)) {
			for (; 1 <= card_index[i];) {
				// 顺子的牌
				if ((card_index[i + 1] >= 1) && (card_index[i + 2] >= 1)) {
					i += 2;
					break;
				}
				else {
					return i;
				}
			}

		}
		else {
			return i;
		}
	}
	return -1;
}

int SRMahjongCard::analysis(void)
{

	for (int i = 0; i < 3; ++i) {
		const int& pos = i * 9;
		if (0 != analysisHuaPai(cardIndex_ + pos, cardIndex_ + pos + 9, cardIndexState_ + pos))
			return -1;
	}



	return 0;
}



int SRMahjongCard::analysisHuaPai(const BYTE * cardIndexBegin, 
	const BYTE * cardIndexEnd, BYTE* outCardIndexState) const {

	if (outCardIndexState == nullptr || cardIndexBegin == nullptr || cardIndexEnd == nullptr)
		return -87;

	// 计算大小
	const int& data_size = cardIndexEnd - cardIndexBegin;
#ifdef _DEBUG											 
	if (data_size > 10) {
		std::cout << __FUNCTION__ << " warning: cache overflow!" << std::endl;
	}
#endif

	// 将对应的返回区重置
	memset(outCardIndexState, 0, data_size);

	// 对整体结构进行分析
	for (int i = 0; i < data_size; ++i) {
		if (cardIndexBegin[i] == 0)
			continue;

		outCardIndexState[i] |= 0x00;

		// 判断顺子
		if ((cardIndexBegin[i] >= 1)
			&& (cardIndexBegin[i + 1] >= 1) && (cardIndexBegin[i + 2] >= 1)) {
				outCardIndexState[i] |= 0x08;
				outCardIndexState[i + 1] |= 0x08;
				outCardIndexState[i + 2] |= 0x08;
		}

		// 对子
		if (cardIndexBegin[i] == 2) {
			outCardIndexState[i] |= 0x01;
		}
		// 刻
		else if (cardIndexBegin[i] == 3) {
			outCardIndexState[i] |= 0x02;
		}
		// 杠
		else if (cardIndexBegin[i] == 4) {
			outCardIndexState[i] |= 0x04;
		}
	}
	
	return 0;
}

