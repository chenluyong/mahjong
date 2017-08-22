#include "SRAnalysis.h"
#include "srmjglobal.h"



SRAnalysis::SRAnalysis()
{
}


SRAnalysis::~SRAnalysis()
{
}

int SRAnalysis::isWin(const unsigned char * data, unsigned char count) {
	// 检查牌数
	if ((count < 2) || (count > MAX_COUNT) || ((count - 2) % 3 != 0))
		return -1;

	unsigned char card_index[MAX_INDEX];

	// 数据转换为索引模式(如果数据有误，可能会造成数组越界)
	for (int i = 0; i < count; ++i)
		card_index[switchToCardIndex(data[i])]++;
			   

	isWin(card_index);

	return 0;
}


int SRAnalysis::isWin(const unsigned char _cardIndex[], int _duiIndex)
{
	//计算数目				 
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


int SRAnalysis::getKeAndShun(const unsigned char _cardIndex[]) {
	// 不修改原值
	BYTE cbCardIndex[MAX_INDEX] = {};
	memcpy(cbCardIndex, _cardIndex, sizeof(_cardIndex[0])* MAX_INDEX);

	// 组合数量变量声明		
	BYTE cbKindItemCount = 0;

	// 拆分分析
	for (BYTE i = 0; i < MAX_INDEX; i++) {
		//同牌判断
		if (cbCardIndex[i] == 3) {
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

unsigned char SRAnalysis::switchToCardIndex(unsigned char cbCardData) {
	return ((cbCardData&MASK_COLOR) >> 4) * 9 + (cbCardData&MASK_VALUE) - 1;
}

unsigned char SRAnalysis::switchToCardData(unsigned char cbCardIndex) {
	return ((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1);
}

int SRAnalysis::isTing(const unsigned char * data, unsigned char count, 
	const unsigned char * out_data, unsigned char* out_count) {


	return 0;
}
