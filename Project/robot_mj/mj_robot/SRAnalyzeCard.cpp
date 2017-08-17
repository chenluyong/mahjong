#include "SRAnalyzeCard.h"

#include <time.h>
#include <iostream>


#define DEBUG(info) std::cout << info << std::endl;

SRAnalyzeCard::SRAnalyzeCard()
{
}


SRAnalyzeCard::~SRAnalyzeCard()
{
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

int SRAnalyzeCard::getOutCard(unsigned char * _out_card) {
	if (_out_card == nullptr)
		return -1;


	srand((unsigned int)time(0));

	DEBUG("分析主角牌型,获得需要打出的牌")
	
	// 抉择牌分析
	do {
		if (rand() % 2) {
			DEBUG("出现抉择牌型(1234, 1233)")	
			DEBUG("检查是否有抉择牌外的牌可供出牌")							   
			if (rand() % 2) {

				DEBUG("未发现，对抉择牌进行深入分析\n")
					DEBUG("-----------begin----------")
					DEBUG("查看最近5张打出的麻将牌")	

				if (rand() % 2) {
					DEBUG("发现熟牌，优先跟熟牌")
				}
				DEBUG("获得当前回合数")
														

				DEBUG("------------end-----------")
			}
			else {
				DEBUG("发现抉择牌外的弃用麻将牌")
				DEBUG("退出抉择牌分析")

				std::cout << std::endl << std::endl;
				break;
			}
		}
	} while (false);

	// 分析自己的牌，得到需要打出的牌
	   // 当出现例如 1234 这种牌型 遇到抉择的时候，分析在场所有的对子，保证补被碰是最优选择

	   // 出牌分析

	   // 如遇抉择牌型
	   // 有熟张出熟张
	   // 走生张时应再判断
	   // 1. 生张  2. 

	return 0;
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

