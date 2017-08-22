// srmjrobot.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "srmjrobot.h"
#include "SRMahjong.h"	  
#include "SRMahjongPool.h"
#include "SRAnalysis.h"


SRRobot::SRRobot(void) {
	// 数据初始化
	isTing_ = -1;
	invisibleMahjongPool_ = nullptr;
	visibleMahjongPool_ = nullptr;
	direction_ = enDirection::None;
	for (int i = 0; i < sizeof(mahjong_) / sizeof(mahjong_[0]); ++i)
		mahjong_[i] = nullptr;
}

SRRobot::~SRRobot(void) {
}

int SRRobot::touchCard(int direction) {
	SRMahjong* pmj = mahjong_[direction_];

	if (invisibleMahjongPool_ == nullptr || pmj == nullptr)
		return -1;

	// 摸牌
	invisibleMahjongPool_->touchCard(pmj, direction);

	return 0;
}

int SRRobot::getOutCard(unsigned char * out_card, unsigned char * out_card_count) {
	if (out_card == nullptr || out_card_count == nullptr)
		return -87;

	SRMahjong* pmj = nullptr;
	if (direction_ != enDirection::None)
		pmj = mahjong_[direction_];

	if (pmj == nullptr)
		return -1;


	if (isTing_ <= 0) {
		// 判听
		isTing_ = SRAnalysis::isTing(pmj->data(), pmj->size());

	}

	if (isTing_ > 0) {
		// 听牌算法
		
		return WIK_LISTEN;
	}

	do {
		// 有杠先杠
		for (unsigned char i = 0, s = 0; i < MAX_INDEX; ++i) {
			s = pmj->getIndex(i);
			if (s == 4) {
				// 检测若在听牌的情况下，如果杠了 是否还能听牌, 杠了是不是牌型会变的更差
				int i = 0;

				
				*out_card = SRAnalysis::switchToCardData(s);
				*out_card_count = 4;
				return WIK_GANG;
			}
		}

		// 获得所有孤立2个空位的不连续单牌
		int index = pmj->getIntervalTwo();
		if (index > 0) {
			*out_card = SRAnalysis::switchToCardData(index);
			break;
		}


		// 获得所有可供打出的不连续单张
		std::vector<int> vec_index;
		for (index = 0; (index = pmj->getIntervalOne(index)) != 0;) {
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
			*out_card = SRAnalysis::switchToCardData(index);
			break;
		}

	} while (false);


	return WIK_NULL;
}

int SRRobot::getAction(enDirection drc, unsigned char card) {
	
	return 0;
}

void SRRobot::setDirection(enDirection drc) {
	direction_ = drc;
}

SRRobot::enDirection SRRobot::getDirection(void) const {
	return direction_;
}

void SRRobot::setVisibleMahjongPool(SRMahjongPool * pool) {
	visibleMahjongPool_ = pool;
}

void SRRobot::setInvisibleMahjongPool(SRMahjongPool * pool) {
	invisibleMahjongPool_ = pool;
}

const SRMahjong * SRRobot::getMahjong(void) const {
	return mahjong_[direction_];
}

const SRMahjong * SRRobot::getMahjong(enDirection drc) const {
	return mahjong_[drc];
}

void SRRobot::setMahjong(enDirection drc, SRMahjong * mahjong) {
	mahjong_[drc] = mahjong;
}
