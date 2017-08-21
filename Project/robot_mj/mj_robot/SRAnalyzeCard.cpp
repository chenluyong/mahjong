#include "SRAnalyzeCard.h"
#include "SRCard.h"

#include <time.h>
#include <iostream>
#include <vector>


#define DEBUG(info) std::cout << info << std::endl;

SRAnalyzeCard::SRAnalyzeCard()
{
}


SRAnalyzeCard::~SRAnalyzeCard()
{
}

void SRAnalyzeCard::reset(void)
{
	isTing_ = false;
}

void SRAnalyzeCard::setGameCardForShow(stCardData _data)
{
	showCardData_ = _data;
}

void SRAnalyzeCard::setGameCardForHide(stCardData _data)
{
	showCardData_ = _data;
}

void SRAnalyzeCard::setGameCardForPlayer(enDirection _dec, stCardData _data)
{
	mapCardData_[_dec] = SRCardMagic(_data);
}

void SRAnalyzeCard::setGameProtagonists(enDirection _dec)
{
	protagonists_ = _dec;
}

int SRAnalyzeCard::getOutCard(unsigned char _in_card, unsigned char * _out_card) {

	if (_out_card == nullptr)
		return -1;


	// 获得当前牌型数据
	BYTE card_index[MAX_INDEX] = {};
	mapCardData_[protagonists_].getCardIndexData(card_index);

	
	int arg1 = 0,arg2 = 0;
	// 检查听牌
	if (!isTing_ && (0 <= (arg1 = SRCardMagic::isTing(card_index)))) {
		arg2 = SRCardMagic::isTing(card_index, arg1);
		vecTingIndex_.push_back(arg1);
		if (arg2 >= 0)
			vecTingIndex_.push_back(arg2);
		isTing_ = true;
	}

	// 将牌插入到牌型中
	mapCardData_[protagonists_].addCardData(_in_card);
	mapCardData_[protagonists_].getCardIndexData(card_index);
	SRCard card(card_index);

	// 检查胡牌
	if (isTing_ && 0 == card.isWin()) {
		return WIK_CHI_HU;
	}



	// 不能胡，那么就开始套路打牌。
	do {

		// 听牌牌型 进入特殊的听牌牌型分析
		if (isTing_) {
			
			
			// 确定要听的牌在牌堆里，或不是对家的刻字或顺子，
			// 如果是，则尽可能的换听牌牌型，无法换牌则以不点炮为上
		}

		// 有杠先杠
		for (int i = 0; i < MAX_INDEX; ++i) {
			if (card[i] == 4) {
				// 若在听牌的情况下，如果杠了 是否还能听牌

				*_out_card = SRCardMagic::switchToCardData(i);
				return WIK_GANG;
			}
		}
		
		// 获得单张的番牌
		int index = card.getFanPaiOne();
		if (index > 0) {
			*_out_card = SRCardMagic::switchToCardData(index);
			break;
		}
		

		// 获得所有孤立2个空位的不连续单牌
		index = card.getIntervalTwo();
		if (index > 0) {
			*_out_card = SRCardMagic::switchToCardData(index);
			break;
		}
	

		// 获得所有可供打出的不连续单张
		std::vector<int> vec_index;
		for (index = 0; (index = card.getIntervalOne(index)) != 0;) {
			vec_index.push_back(index);
		}

		// 检查是否有单牌可出
		if (!vec_index.empty()) {
			// 遍历检查幺九牌型
			for (auto x : vec_index) {
				x %= 9;
				if (x <= 2 || x >= 7) {
					index = x;
					break;
				}
			}
			*_out_card = SRCardMagic::switchToCardData(index);
			break;
		}


		// 进行到这里说明出现抉择类牌型
		// 开启胡牌出法 分析场上局势
		*_out_card = card.getOutCard();
		

	} while (false);


	mapCardData_[protagonists_].delCardData(*_out_card);
	return 0;
}

int SRAnalyzeCard::getAction(unsigned char _in_card)
{
	BYTE card_index[MAX_INDEX] = {};
	mapCardData_[protagonists_].getCardIndexData(card_index);

	// 判断胡牌
	if (isTing_) {
		card_index[SRCardMagic::switchToCardIndex(_in_card)]++;
		if (0 == SRCardMagic::isWin(card_index))
			return WIK_CHI_HU;
		return WIK_NULL;
	}


	// 询问吃碰杠
	



	return WIK_NULL;
}

void SRAnalyzeCard::analyzeGamePlayer(enDirection _in_dec, stCardData * _out_dynamic, stCardData * _out_static)
{
}

int SRAnalyzeCard::analyzeProtagonists(void)
{
	//unsigned char* card = mapCardData_.at(protagonists_);
	//unsigned char card_count = mapCardData_.at(protagonists_).size;
	
	// 判断牌的数量
	//if ((card_count < 2) || (card_count > MAX_COUNT) || ((card_count - 2) % 3 != 0))
	//	return -1;

	// 牌型转换成索引模式

	return 0;
}

void SRAnalyzeCard::analyzeGamePlayerForChain(void)
{
}

