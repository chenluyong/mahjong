// SRMahjongRobot.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "SRMahjong.h"
#include "SRMahjongRobot.h"
#include "SRMahjongAnalysis.h"

#include <iostream>

#include <time.h>


// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 SRMahjongRobot.h
SRMahjongRobot::SRMahjongRobot(void) {
	direction_ = enDirection::None;
	memset(enemyDirection_, 0, sizeof(enemyDirection_));
	roundNumber_ = 0;

	analysisLogic_ = new SRMahjongAnalysis();

	// 牌池中的麻将
	visibleMahjongPool_ = nullptr;
	// 牌墙上的麻将
	invisibleMahjongPool_ = nullptr;

	

	// 所有方位的麻将		
	memset(mahjong_, 0, sizeof(mahjong_));
}

int SRMahjongRobot::touchCard(unsigned char card) {
	SRMahjong* pmj = mahjong_[direction_];

	if (pmj == nullptr)
		return -1;

	// 轮次递增
	roundNumber_++;

	// 摸牌
	return pmj->addCardData(card);
}

int SRMahjongRobot::getOutCard(unsigned char * out_card, unsigned char * out_card_count) {
	if (out_card == nullptr || out_card_count == nullptr)
		return -87;

	// 检查角色自身
	SRMahjong* pmj = nullptr;
	if (direction_ != enDirection::None)
		pmj = mahjong_[direction_];
	if (pmj == nullptr)
		return -1;
	if (analysisLogic_->getMahjong() != pmj)
		analysisLogic_->setMahjong(pmj);

	srand((unsigned int)time(0));

	


	return 0;
}

int SRMahjongRobot::getAction(enDirection drc, unsigned char card) {
	return 0;
}

void SRMahjongRobot::setDirection(enDirection drc) {
	direction_ = drc;
	// 将主角麻将数据设置给分析类
	if (mahjong_[direction_] != nullptr)
		analysisLogic_->setMahjong(mahjong_[direction_]);
}

SRMahjong * SRMahjongRobot::getMahjong(void) {
	if (direction_ == enDirection::None)
		return nullptr;

	return mahjong_[direction_];
}

const SRMahjong * SRMahjongRobot::getMahjong(enDirection drc) const {
	if (drc == enDirection::None)
		return nullptr;

	return mahjong_[drc];
}

void SRMahjongRobot::setMahjong(enDirection drc, SRMahjong * mahjong) {
	if (drc == enDirection::None)
		return;

	// 设置麻将
	mahjong_[drc] = mahjong;

	// 将主角麻将数据设置给分析类
	if (drc == direction_)
		analysisLogic_->setMahjong(mahjong_[drc]);
}

void SRMahjongRobot::setEnemy(enDirection drc, int isEnemy) {
	if (drc == enDirection::None)
		return;

	enemyDirection_[drc] = isEnemy;
}
