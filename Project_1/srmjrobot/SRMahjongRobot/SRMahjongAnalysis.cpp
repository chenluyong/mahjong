#include "SRMahjongAnalysis.h"
#include "SRMahjong.h"
#include <iostream>


SRMahjongAnalysis::SRMahjongAnalysis(void)
{
	memset(cardIndexState_, 0, sizeof(cardIndexState_));
}


SRMahjongAnalysis::~SRMahjongAnalysis(void)
{
}




int SRMahjongAnalysis::analysis(void) {
	if (mahjong_ == nullptr)
		return -1;

	const BYTE* card_index = mahjong_->getIndex(0);

	return analysis(card_index, card_index + MAX_INDEX);
}

int SRMahjongAnalysis::analysis(const BYTE * _cardIndexBegin, const BYTE * _cardIndexEnd) {
	if (_cardIndexBegin == nullptr || _cardIndexEnd == nullptr)
		return -87;

	memset(cardIndexState_, 0, sizeof(cardIndexState_));

	int data_size = _cardIndexEnd - _cardIndexBegin;
	for (int i = 0; i < data_size / 9; ++i) {
		const int& pos = i * 9;
		if (0 != analysisHuaPai(_cardIndexBegin + pos, _cardIndexBegin + pos + 9, cardIndexState_ + pos))
			return -1;
	}
	return 0;
}

int SRMahjongAnalysis::analysisHuaPai(const BYTE * cardIndexBegin,
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


