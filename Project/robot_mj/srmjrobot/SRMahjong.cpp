#include "SRMahjong.h"
#include "SRAnalysis.h"

#include <iostream>

#include <algorithm>



SRMahjong::SRMahjong(unsigned char cardData[], unsigned char cardCount) {
	memset(cardIndex_, 0, sizeof(cardIndex_));
	memset(cardData_, 0, sizeof(cardData_));
	cardCount_ = cardCount;
	
	lastTouchCard_ = 0;

#ifdef _DEBUG
	int card_size = cardCount * sizeof(unsigned char);
	if (card_size > sizeof(cardData_)) {
		std::cout << __FUNCTION__ << "The data of crossing the line." << std::endl;
	}
#endif

	memcpy(cardData_, cardData, cardCount_ * sizeof(unsigned char));

	// 数据转换为索引模式(如果数据有误，可能会造成数组越界)
	for (int i = 0; i < cardCount_; ++i)
		cardIndex_[SRAnalysis::switchToCardIndex(cardData[i])]++;
}

SRMahjong::~SRMahjong() {
}


unsigned char SRMahjong::have(unsigned char card) {
	return cardIndex_[SRAnalysis::switchToCardIndex(card)];
}

void SRMahjong::sort(void) {
	std::sort(cardData_, cardData_ + MAX_COUNT - 1);
}

void SRMahjong::upCardIndex(void)
{
	memset(cardIndex_, 0, sizeof(cardIndex_));
	for (int i = 0; i < sizeof(cardData_) / sizeof(cardData_[0]); ++i) {
		int idx = SRAnalysis::switchToCardIndex(cardData_[i]);
		if (cardData_[i] == 0)
			continue;

		// 防止idx下标为-1
		if (idx >= 0 && idx < MAX_INDEX)
			cardIndex_[idx]++;
		else
			std::cout << "warning:" << __FUNCTION__ << std::endl;
	}
}

unsigned char SRMahjong::length(void) 
{		
	//数目统计
	BYTE cbCardCount = 0;
	for (BYTE i = 0; i<MAX_INDEX; i++)
		cbCardCount += cardIndex_[i];

	return cbCardCount;
}

void SRMahjong::addCard(unsigned char card)
{
	cardData_[MAX_COUNT - 1] = card;
	lastTouchCard_ = card;
	cardCount_ += 1;

	this->sort();
	this->upCardIndex();
}

void SRMahjong::delCard(unsigned char card)
{
	// 这里还需要单步调试一下
	auto c = std::find(cardData_, cardData_ + MAX_COUNT, card);

	if (c != nullptr)
		*c = 0;

	cardCount_ -= 1;

	this->sort();
	this->upCardIndex();
}



int SRMahjong::getFanPaiOne(int _indexBegin) const {
	if (_indexBegin == 0)
		_indexBegin = MAX_INDEX - MAX_FAN;
	else if (_indexBegin < MAX_INDEX - MAX_FAN || _indexBegin >= MAX_INDEX)
		return 0;

	for (int i = _indexBegin; i < MAX_INDEX; ++i) {
		if (cardIndex_[i] == 1) {
			return i;
		}
	}

	return 0;
}



int SRMahjong::getIntervalOne(int _indexBegin) const {
	BYTE card_index[MAX_INDEX] = {};
	memcpy(card_index, cardIndex_, sizeof(cardIndex_[0])*MAX_INDEX);

	for (int i = _indexBegin / 9; i <= MAX_INDEX / 9; ++i) {
		const int& INDEX_END = i * 9 + 9, &INDEX_BEGIN = i * 9;

		for (int index_begin = INDEX_BEGIN; index_begin < INDEX_END; ++index_begin) {

			// 没有牌 或者 牌在指定的范围之前 都选择跳过
			if (card_index[index_begin] == 0 || index_begin <= _indexBegin)
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

int SRMahjong::getIntervalTwo(int _indexBegin) const {

	// 不改动原数据
	BYTE card_index[MAX_INDEX] = {};
	memcpy(card_index, cardIndex_, sizeof(cardIndex_[0])*MAX_INDEX);

	for (int i = _indexBegin / 9; i <= MAX_INDEX / 9; ++i) {
		const int& INDEX_END = i * 9 + 9;
		const int& INDEX_BEGIN = i * 9;

		for (int index_begin = INDEX_BEGIN; index_begin < INDEX_END; ++index_begin) {

			// 没有牌 或者 牌在指定的范围之前 都选择跳过
			if (card_index[index_begin] == 0 || index_begin < _indexBegin)
				continue;

			const int& arg1 = index_begin - 1, &arg2 = index_begin - 2;
			const int& arg3 = index_begin + 1, &arg4 = index_begin + 2;

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

bool isValidCard(unsigned char cbCardData)
{
	return SRAnalysis::isValidCard(cbCardData);
}
