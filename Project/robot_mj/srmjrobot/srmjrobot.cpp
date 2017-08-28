// srmjrobot.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "srmjrobot.h"
#include "SRMahjong.h"	  
#include "SRMahjongPool.h"
#include "SRAnalysis.h"


SRRobot::SRRobot(void) {
	reset();
	std::cout << "hello robot!" << std::endl;
}

SRRobot::~SRRobot(void) {
}

int SRRobot::touchCard(unsigned char card) {
	SRMahjong* pmj = mahjong_[direction_];

	if (pmj == nullptr)
		return -1;

	// 摸牌
	pmj->addCard(card);
	return 0;
}

int SRRobot::getOutCard(unsigned char * out_card, unsigned char * out_card_count) {
	// 数据校验
	if (out_card == nullptr || out_card_count == nullptr)
		return -87;

	// 检查角色自身
	SRMahjong* pmj = nullptr;
	if (direction_ != enDirection::None)
		pmj = mahjong_[direction_];
	if (pmj == nullptr)
		return -1;

	// 判断自摸
	if (0 == SRAnalysis::isWin(pmj->data(), pmj->length())) {
		return WIK_CHI_HU;
	}


	// 听牌分析
	unsigned char temp_discard[MAX_COUNT];
	memset(temp_discard, 0, sizeof(temp_discard));
	if (isTing_ > 0 || (isTing_ = SRAnalysis::isTing(pmj->data(), 
		pmj->length(), temp_discard, arrTing_, &numTing_)) > 0) {
		// 检查所听牌型场上情况	
		for (int i = 0; i < numTing_; ++i) {
			// 检验数组
			if (arrTing_[i] == 0)
				continue;

			// 检查可查
			if (invisibleMahjongPool_ == nullptr) {
				// 查看麻将是否在牌墙之上
				int pool_idx = invisibleMahjongPool_->find(arrTing_[i]);

				if (pool_idx < 0) {
					// 不在牌墙之上，检测是否在牌池之中
					//visibleMahjongPool_->find(arrTing_[i]);

					// 检查玩家手中是否存有主角所听的牌
					for (int i = 0; i < numTing_; ++i) {

						// 检测是否在玩家手中
						for (int player = enDirection::South; player <= enDirection::East; ++player) {

							// 检查可查
							if (mahjong_[player] == nullptr)
								continue;

							if (0 < mahjong_[player]->have(arrTing_[i])) {
								// 检测他是否成将或成顺刻
								if (false) {
									arrTing_[i] = 0;
								}
								else {
									break;
								}
							}
						}
					}
				}

			}
			else {
				// 检查牌池之中有几张可听牌型
				int num = visibleMahjongPool_->have(arrTing_[i]);
				num += pmj->have(arrTing_[i]);
				if (num == 4) {
					// 无法胡牌
				}
			}
			
			
		}

		// 检查是否已成死牌
		{
			int temp_die = 0;
			for (int i = 0; i < numTing_; ++i)
				if (arrTing_[i] == 0)
					++temp_die;

			if (temp_die == numTing_) {
				// 说明已经无牌可听，现在以不点炮为主
			}
		}
		
		
			


		*out_card = temp_discard[0];
		*out_card_count = 1;
		pmj->delCard(*out_card);

		return WIK_LISTEN;
	}


	// 套路出牌
	do {
		// 有杠先杠
		for (unsigned char i = 0, s = 0; i < MAX_INDEX; ++i) {
			s = pmj->index(i);
			if (s == 4) {
				// 检测若在听牌的情况下，如果杠了 是否还能听牌, 杠了是不是牌型会变的更差
				int i = 0;

				*out_card = SRAnalysis::switchToCardData(s);
				*out_card_count = 4;

				for(int i = 0; i < *out_card_count; ++i)
					pmj->delCard(*out_card);

				return WIK_GANG;
			}
		}


		//// 有单独的风牌则打风牌
		std::vector<int> vec_index;
		//{
		//	for (int idx = 0; (idx = pmj->getFanPaiOne(idx)) != -1;++idx) {
		//		vec_index.push_back(idx);
		//	}
		//	if (!vec_index.empty()) {
		//		int i = rand() % vec_index.size();
		//		int temp = vec_index.at(i);
		//		// 这里可以复杂一点
		//		for (int i = 0; i < 4; ++i) {
		//			if (mahjong_[i] == nullptr && i != (int)direction_)
		//				continue;

		//			// 风有好几张，先打别人能碰的，以达到快速切牌的效果
		//			for (auto idx : vec_index) {
		//				if (2 <= mahjong_[i]->have(idx)) {
		//					temp = idx;
		//				}
		//			}
		//		}

		//		*out_card = SRAnalysis::switchToCardData(temp);
		//		*out_card_count = 1;
		//		break;
		//	}
		//}

		// 获得所有孤立2个空位的不连续单牌  [122334 7] 
		//{

		//	vec_index.clear();
		//	for (int idx = 0; (idx = pmj->getIntervalTwo(idx)) != -1;++idx) {
		//		if (pmj->index((unsigned char)idx) >= 2) {
		//			// 判断是否唯一将牌
		//			continue;
		//		}
		//		vec_index.push_back(idx);
		//	}
		//	if (!vec_index.empty()) {
		//		int i = rand() % vec_index.size();
		//		int temp_index = vec_index.at(i);

		//		// 遍历检查幺九牌型
		//		for (auto x : vec_index) {
		//			const int& temp = x % 9;
		//			if (temp <= 1 || temp >= 7) {
		//				temp_index = x;
		//				break;
		//			}
		//		}

		//		*out_card = SRAnalysis::switchToCardData(temp_index);
		//		*out_card_count = 1;
		//		break;
		//	}

		//}

		


		// 获得所有可供打出的不连续单张 [1 3 567]   
		//{
		//	vec_index.clear();
		//	for (int idx = 0; (idx = pmj->getIntervalOne(idx)) != -1;++idx) {
		//		if (pmj->index((unsigned char)idx) >= 2) {
		//			// 判断是否唯一将牌
		//			continue;
		//		}
		//		vec_index.push_back(idx);
		//	}
		//	// 检查是否有单牌可出
		//	if (!vec_index.empty()) {
		//		// ![遗留] 确定这个单牌不是将牌及其以上，或这不是唯一将牌

		//		int i = rand() % vec_index.size();
		//		int temp_index = vec_index.at(i);
		//		// 遍历检查幺九牌型
		//		for (auto x : vec_index) {
		//			const int& temp = x % 9;
		//			if (temp <= 1 || temp >= 7) {
		//				temp_index = x;
		//				break;
		//			}
		//		}
		//		*out_card = SRAnalysis::switchToCardData(temp_index);
		//		*out_card_count = 1;
		//		break;
		//	}
		//}


		// 拆掉 1233 1223  7789 这样的将牌,成为无将可听型
		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getShunDopant(idx)) != -1; ++idx) {
				vec_index.push_back(idx);
			}
			// 检查是否有牌可出
			if (!vec_index.empty()) {
				int i = rand() % vec_index.size();
				int temp_index = vec_index.at(i);
				// 遍历检查幺九牌型
				for (auto x : vec_index) {
					const int& temp = x % 9;
					if (temp <= 1 || temp >= 7) {
						temp_index = x;
						break;
					}
				}
				*out_card = SRAnalysis::switchToCardData(temp_index);
				*out_card_count = 1;
				break;
			}
			
		}



		// 若函数进行到这里，说明牌型比较复杂了，需要加入权重概念进行深度分析
		*out_card = pmj->getLastTouchCard();
		*out_card_count = 1;
	} while (false);

	// 删除打出的牌
	for (int i = 0; i < (*out_card_count); ++i)
		pmj->delCard(*out_card);

	return WIK_NULL;
}

int SRRobot::getAction(enDirection drc, unsigned char card) {
	// 检验参数
	if (!SRAnalysis::isValidCard(card) || drc == enDirection::None)
		return -87;

	// 配备主角
	SRMahjong* pmj = nullptr;
	if (direction_ != enDirection::None)
		pmj = mahjong_[direction_];

	if (pmj == nullptr)
		return -1;

	// 检验可胡
	SRMahjong temp_mj(*pmj);
	temp_mj.addCard(card);

	if (0 == SRAnalysis::isWin(temp_mj.data(), temp_mj.length())) {
		return WIK_CHI_HU;
	}

	// 检验碰杠（四川麻将只有碰杠）
	for (unsigned char i = 0; i < MAX_INDEX; ++i) {
		if (pmj->index(i) == 3) {
			// 杠牌逻辑分析
			return WIK_GANG;
		}
		else if (pmj->index(i) == 2) {
			// 碰牌逻辑分析
			return WIK_PENG;
		}
	}
	return WIK_NULL;
}

void SRRobot::reset(void) {	
	// 主角方位
	direction_ = enDirection::None;

	// 牌池中的麻将
	visibleMahjongPool_ = nullptr;
	// 牌墙上的麻将
	invisibleMahjongPool_ = nullptr;

	// 所有方位的麻将
	for (int i = 0; i < sizeof(mahjong_) / sizeof(mahjong_[0]); ++i)
		mahjong_[i] = nullptr;

	// 牌型是否可听
	isTing_ = -1;
	// 听的牌
	memset(arrTing_, 0, sizeof(arrTing_));
	// 可听的数量
	numTing_ = 0;
}

void SRRobot::setDirection(enDirection drc) {
	direction_ = drc;
}

enDirection SRRobot::getDirection(void) const {
	return direction_;
}

void SRRobot::setVisibleMahjongPool(SRVisibleMahjongPool * pool) {
	visibleMahjongPool_ = pool;
}

void SRRobot::setInvisibleMahjongPool(SRInvisibleMahjongPool * pool) {
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

