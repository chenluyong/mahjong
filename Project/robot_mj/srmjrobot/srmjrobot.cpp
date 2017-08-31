// srmjrobot.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "srmjrobot.h"
#include "SRMahjong.h"	  
#include "SRMahjongPool.h"
#include "SRAnalysis.h"

#include <time.h>

SRRobot::SRRobot(void) {
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
	roundNumber_ = 0;
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

	pmj->upCardIndex();

	srand((unsigned int)time(0));
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
		{
			for (int idx = 0; (idx = pmj->getFanPaiOne(idx)) != -1;++idx) {
				vec_index.push_back(idx);
			}
			if (!vec_index.empty()) {
				int i = rand() % vec_index.size();
				int temp = vec_index.at(i);
				// 快速切牌判断
				for (int i = 0; i < 4; ++i) {
					if (mahjong_[i] == nullptr && i == (int)direction_)
						continue;

					// 风有好几张，先打别人能碰的，以达到快速切牌的效果
					for (auto idx : vec_index) {
						if (2 <= mahjong_[i]->have(SRAnalysis::switchToCardData(idx))) {
							temp = idx;
						}
					}
				}

				*out_card = SRAnalysis::switchToCardData(temp);
				*out_card_count = 1;
				break;
			}
		}

		// 获得所有孤立2个空位的不连续单牌  [122334 7] 
		{

			vec_index.clear();
			for (int idx = 0; (idx = pmj->getIntervalTwo(idx)) != -1;++idx) {
				if (pmj->index((unsigned char)idx) >= 2) {
					// 判断是否唯一将牌
					continue;
				}
				vec_index.push_back(idx);
			}
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

				// 快速切牌判断
				for (int i = 0; i < 4; ++i) {
					if (mahjong_[i] == nullptr && i == (int)direction_)
						continue;

					// 风有好几张，先打别人能碰的，以达到快速切牌的效果
					for (auto idx : vec_index) {
						if (2 <= mahjong_[i]->have(SRAnalysis::switchToCardData(idx))) {
							temp_index = idx;
						}
					}
				}
				*out_card = SRAnalysis::switchToCardData(temp_index);
				*out_card_count = 1;
				break;
			}

		}


		// 获得所有可供打出的不连续单张 [1 3 567]   
		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getIntervalOne(idx)) != -1;++idx) {
				if (pmj->index((unsigned char)idx) >= 2) {
					// 判断是否唯一将牌
					continue;
				}
				vec_index.push_back(idx);
			}
			// 检查是否有单牌可出
			if (!vec_index.empty()) {
				// ![遗留] 确定这个单牌不是将牌及其以上，或这不是唯一将牌

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


		// [112  223 344]
		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getDanDui(idx)) != -1; ++idx) {
				vec_index.push_back(idx);
			}
			// 检查是否有牌可出
			if (!vec_index.empty()) {

				int i = rand() % vec_index.size();
				int temp_index = vec_index.at(i);

				// 分析最佳出牌
				for (auto v_index : vec_index) {
					// 确定牌型 [122,223]
					unsigned char temp_dui = v_index;
					unsigned char temp_dan = v_index + 1;
					if (pmj->index(v_index) == 1) {
						// [122]
						temp_dui = v_index + 1;
						temp_dan = v_index;
					}

					// 如果这是唯一的将牌，则打单张无遗
					if (vec_index.size() == 1) {
						temp_index = temp_dan;
						break;
					}


					// 如果此牌已经打出了两张则
					if (2 == visibleMahjongPool_->have(SRAnalysis::switchToCardData(temp_dui))) {
						temp_index = temp_dui;
						break;
					}

					// 若是 [122 || 899] 则出单牌
					if (temp_dan % 9 == 0 || temp_dui % 9 == 8) {
						temp_index = temp_dan;
						break;
					}

					// 若其它玩家有可能让我们碰，则出单张。  
					// 例如 A家有 [12357] 我们家的牌是 [455],则打 4
					for (int i = 0; i < 4; ++i) {
						if (mahjong_[i] == nullptr && i == (int)direction_)
							continue;

						if (0 != mahjong_[i]->getIntervalOne(SRAnalysis::switchToCardData(temp_dui))) {
							temp_index = temp_dan;
						}
						
					}

				}



				*out_card = SRAnalysis::switchToCardData(temp_index);
				*out_card_count = 1;
				break;
			}
		}


		// 解决所有非刻非顺的牌
		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getNotShunKe(idx)) != -1; ++idx) {
				vec_index.push_back(idx);
			}
			// 检查是否还有对子
			for (auto iter_begin = vec_index.begin(); iter_begin != vec_index.end();
				++iter_begin) {
				if (*iter_begin == 2) {
					iter_begin = vec_index.erase(iter_begin);
				}
			}
			// 检查是否有牌可出
			if (!vec_index.empty()) {


				int i = rand() % vec_index.size();
				int temp_index = vec_index.at(i);


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

int SRRobot::getNewOutCard(unsigned char * out_card, unsigned char * out_card_count) {
	// 数据校验
	if (out_card == nullptr || out_card_count == nullptr)
		return -87;


	// 检查角色自身
	SRMahjong* pmj = nullptr;
	if (direction_ != enDirection::None)
		pmj = mahjong_[direction_];
	if (pmj == nullptr)
		return -1;

	pmj->upCardIndex();
	srand((unsigned int)time(0));



	// 判断自摸
	if (0 == SRAnalysis::isWin(pmj->data(), pmj->length())) {
		return WIK_CHI_HU;
	}


	int ret = WIK_NULL;

	// 判听
	unsigned char temp_discard[MAX_COUNT];
	memset(temp_discard, 0, sizeof(temp_discard));
	if (isTing_ > 0 || (isTing_ = SRAnalysis::isTing(pmj->data(),
		pmj->length(), temp_discard, arrTing_, &numTing_)) > 0) {

		*out_card = SRAnalysis::switchToCardData(temp_discard[0]);
		*out_card_count = 1;
		ret = WIK_LISTEN;
	}


	// 分析
	do {

		
		// 有杠先杠
		{
			for (unsigned char i = 0, s = 0; i < MAX_INDEX; ++i) {
				s = pmj->index(i);
				if (s == 4) {
					// 检测若在听牌的情况下，如果杠了 是否还能听牌, 杠了是不是牌型会变的更差
					int i = 0;

					*out_card = SRAnalysis::switchToCardData(s);
					*out_card_count = 4;


					return WIK_GANG;
				}
			}
		}
		

		// 有单独的风牌则打风牌
		{	   
			std::vector<int> vec_index;
			for (int idx = 0; (idx = pmj->getFanPaiOne(idx)) != -1; ++idx) {
				vec_index.push_back(idx);
			}
			if (!vec_index.empty()) {
				int i = rand() % vec_index.size();
				int temp = vec_index.at(i);
				// 快速切牌判断
				for (int i = 0; i < 4; ++i) {
					if (mahjong_[i] == nullptr && i == (int)direction_)
						continue;

					// 风有好几张，先打别人能碰的，以达到快速切牌的效果
					for (auto idx : vec_index) {
						if (2 <= mahjong_[i]->have(SRAnalysis::switchToCardData(idx))) {
							temp = idx;
						}
					}
				}

				*out_card = SRAnalysis::switchToCardData(temp);
				*out_card_count = 1;
				break;
			}
		}


		// 分析花牌
		{
			// 获得牌型
			unsigned char card_index[MAX_INDEX] = {};
			for (int i = 0; i < MAX_INDEX; ++i)
				card_index[i] = pmj->index(i);

			
			// 开始分析
			{
				std::vector<int> vec_index;

				// 牌型解析
				{
					for (int i = 0; i < 3; ++i) {
						// 存放下标的数组
						unsigned char temp_card_index[MAX_COUNT] = {};
						char temp_count = 0;
						// 分析当前区间的花牌
						SRAnalysis::analysisHuaPai(&card_index[i * 9], &card_index[i * 9 + 9],
							temp_card_index, &temp_count);

						// 将花牌的下标存入容器
						for (int n = 0; n < temp_count; ++n)
							vec_index.push_back((temp_card_index[n] + i * 9));
					}
				}


				// 分析最佳出牌
				{
					if (!vec_index.empty()) {
						int temp_i = rand() % vec_index.size();
						int temp_index = vec_index.at(temp_i);


						// 转移连牌 如 12  34 此类
						std::vector<int> vec_shun_index;
						{
							for (int i = 0; i < vec_index.size(); ++i) {
								// 若下标[i+1] 与 下标 [i] 相邻，则转移
								if (i != 0 && 
									vec_index.at(i - 1) + 1 == vec_index.at(i)) {
									vec_shun_index.push_back(vec_index.at(i));
									vec_shun_index.push_back(vec_index.at(i - 1));
									vec_index.erase(vec_index.begin() + i);
									vec_index.erase(vec_index.begin() + i - 1);
								}
							}
							// 默认出 12  34 顺牌
							if (!vec_shun_index.empty()) {
								temp_index = vec_shun_index.at(rand() % vec_shun_index.size());
							}
						}

						// 转移将牌
						std::vector<int> vec_double_index;
						{
							for (auto iter = vec_index.begin();
								iter != vec_index.end();) {

								if (2 <= pmj->index(*iter)) {
									vec_double_index.push_back(*iter);
									iter = vec_index.erase(iter);
								}
								else {
									++iter;
								}
							}
							// 若将牌不是唯一且，没有散顺可出，则默认出将牌
							if (vec_double_index.size() > 1 && vec_shun_index.empty())
								temp_index = vec_double_index.at(
									rand() % vec_double_index.size());
						}
						

						// 如果还有单牌，则默认出单牌
						if (!vec_index.empty()) {
							temp_index = vec_index.at(
								rand() % vec_index.size());
						}

						// 判断快速切牌
						if (roundNumber_ <= 6 && roundNumber_ >= 0) {
							// 快速切牌判断
							int sign_index = 0;
							for (int i = (int)enDirection::South;
								i <= (int)enDirection::East; ++i) {
								if (mahjong_[i] == nullptr || i == (int)direction_)
									continue;

								// 查看玩家拥有的牌型
								for (auto idx : vec_index) {
									if (2 == mahjong_[i]->have(SRAnalysis::switchToCardData(idx))) {
										sign_index = idx;
									}
								}
							}
							// 判断是否有附和切牌标准的牌型
							if (sign_index != 0 && temp_index != sign_index) {
								(*out_card) = SRAnalysis::switchToCardData(sign_index);
								(*out_card_count) = 1;
								break;
							}
						} // end 快速切牌判断结束

						  // 遍历检查幺九牌型
						for (auto x : vec_index) {
							const int& temp = x % 9;
							if (temp <= 1 || temp >= 7) {
								temp_index = x;
								break;
							}
						}


						
						(*out_card) = SRAnalysis::switchToCardData(temp_index);
						(*out_card_count) = 1;
						break;
					}
				}

			}
			
		}


		return -1;
	} while (false);


	//for (int i = 0; i < *out_card_count; ++i)
	//	pmj->delCard(*out_card);

	return WIK_NULL;
}

int SRRobot::analysisOutCard(unsigned char * out_card, unsigned char * out_card_count)
{	
	return getNewOutCard(out_card, out_card_count);

	// 数据校验
	if (out_card == nullptr || out_card_count == nullptr)
		return -87;

	// 检查角色自身
	SRMahjong* pmj = nullptr;
	if (direction_ != enDirection::None)
		pmj = mahjong_[direction_];
	if (pmj == nullptr)
		return -1;
	pmj->upCardIndex();

	srand((unsigned int)time(0));
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


				return WIK_GANG;
			}
		}


		//// 有单独的风牌则打风牌
		std::vector<int> vec_index;
		{
			for (int idx = 0; (idx = pmj->getFanPaiOne(idx)) != -1; ++idx) {
				vec_index.push_back(idx);
			}
			if (!vec_index.empty()) {
				int i = rand() % vec_index.size();
				int temp = vec_index.at(i);
				// 快速切牌判断
				for (int i = 0; i < 4; ++i) {
					if (mahjong_[i] == nullptr && i != (int)direction_)
						continue;

					// 风有好几张，先打别人能碰的，以达到快速切牌的效果
					for (auto idx : vec_index) {
						if (2 <= mahjong_[i]->have(SRAnalysis::switchToCardData(idx))) {
							temp = idx;
						}
					}
				}

				*out_card = SRAnalysis::switchToCardData(temp);
				*out_card_count = 1;
				break;
			}
		}

		// 获得所有孤立2个空位的不连续单牌  [122334 7] 
		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getIntervalTwo(idx)) != -1; ++idx) {
				if (pmj->index((unsigned char)idx) >= 2) {
					// 判断是否唯一将牌
					continue;
				}
				vec_index.push_back(idx);
			}
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

				// 快速切牌判断
				for (int i = 0; i < 4; ++i) {
					if (mahjong_[i] == nullptr && i != (int)direction_)
						continue;

					// 风有好几张，先打别人能碰的，以达到快速切牌的效果
					for (auto idx : vec_index) {
						if (2 <= mahjong_[i]->have(SRAnalysis::switchToCardData(idx))) {
							temp_index = idx;
						}
					}
				}
				*out_card = SRAnalysis::switchToCardData(temp_index);
				*out_card_count = 1;
				break;
			}

		}

		// 获得所有可供打出的不连续单张 [1 3 567]   
		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getIntervalOne(idx)) != -1; ++idx) {
				if (pmj->index((unsigned char)idx) >= 2) {
					// 判断是否唯一将牌
					continue;
				}
				vec_index.push_back(idx);
			}
			// 检查是否有单牌可出
			if (!vec_index.empty()) {
				// ![遗留] 确定这个单牌不是将牌及其以上，或这不是唯一将牌

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


		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getDanDui(idx)) != -1; ++idx) {
				vec_index.push_back(idx);
			}
			// 检查是否有牌可出
			if (!vec_index.empty()) {
				int temp_index = vec_index.at(0);

				// 分析最佳出牌
				for (auto v_index : vec_index) {
					// 确定牌型 [122,223]
					unsigned char temp_dui = v_index;
					unsigned char temp_dan = v_index + 1;
					if (pmj->index(v_index) == 1) {
						// [122]
						temp_dui = v_index + 1;
						temp_dan = v_index;
					}

					// 如果这是唯一的牌，则打单张
					if (vec_index.size() == 1) {
						temp_index = temp_dan;
						break;
					}


					// 如果此牌已经打出了两张在牌池之中则打对牌
					if (2 == visibleMahjongPool_->have(SRAnalysis::switchToCardData(temp_dui))) {
						temp_index = temp_dui;
						break;
					}

					// 若是 [122 || 899] 则出单牌
					if (temp_dan % 9 == 0 || temp_dui % 9 == 8) {
						temp_index = temp_dan;
						break;
					}

					// 若其它玩家有可能让我们碰，则出单张。  
					// 例如 A家有 [12357] 我们家的牌是 [455],则打 4
					for (int i = 0; i < 4; ++i) {
						if (mahjong_[i] == nullptr && i == (int)direction_)
							continue;

						if (0 != mahjong_[i]->getIntervalOne(SRAnalysis::switchToCardData(temp_dui))) {
							temp_index = temp_dan;
						}

					}

				}

				*out_card = SRAnalysis::switchToCardData(temp_index);
				*out_card_count = 1;
				break;
			}
		}

		// 解决所有非刻非顺的牌
		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getNotShunKe(idx)) != -1; ++idx) {
				vec_index.push_back(idx);
			}
			// 检查是否还有对子
			for (auto iter_begin = vec_index.begin(); iter_begin != vec_index.end();
				++iter_begin) {
				if (*iter_begin == 2) {
					iter_begin = vec_index.erase(iter_begin);
				}
			}
			// 检查是否有牌可出
			if (!vec_index.empty()) {
				
					
				int i = rand() % vec_index.size();
				int temp_index = vec_index.at(i);


				*out_card = SRAnalysis::switchToCardData(temp_index);
				*out_card_count = 1;
				break;
			}
		}

		// 若函数进行到这里，说明牌型比较复杂了，需要加入权重概念进行深度分析
		*out_card = pmj->getLastTouchCard();
		*out_card_count = 1;
	} while (false);


	return WIK_NULL;
}

int SRRobot::analysisCard(unsigned char * out_card, unsigned char * out_card_count)
{
	if (out_card == nullptr || out_card_count == nullptr)
		return -87;

	// 检查角色自身
	SRMahjong* pmj = nullptr;
	if (direction_ != enDirection::None)
		pmj = mahjong_[direction_];
	if (pmj == nullptr)
		return -1;
	pmj->upCardIndex();

	srand((unsigned int)time(0));
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


				return WIK_GANG;
			}
		}


		//// 有单独的风牌则打风牌
		std::vector<int> vec_index;
		{
			for (int idx = 0; (idx = pmj->getFanPaiOne(idx)) != -1; ++idx) {
				vec_index.push_back(idx);
			}
			if (!vec_index.empty()) {
				for (auto temp : vec_index) {
					*out_card = SRAnalysis::switchToCardData(temp);
					out_card++;
					*out_card_count += 1;
				}
			}
		}

		// 获得所有孤立2个空位的不连续单牌  [122334 7] 
		{

			vec_index.clear();
			for (int idx = 0; (idx = pmj->getIntervalTwo(idx)) != -1; ++idx) {
				if (pmj->index((unsigned char)idx) >= 2) {
					// 判断是否唯一将牌
					continue;
				}
				vec_index.push_back(idx);
			}
			if (!vec_index.empty()) {
				for (auto temp : vec_index) {
					*out_card = SRAnalysis::switchToCardData(temp);
					out_card++;
					*out_card_count += 1;
				}
			}

		}




		// 获得所有可供打出的不连续单张 [1 3 567]   
		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getIntervalOne(idx)) != -1; ++idx) {
				if (pmj->index((unsigned char)idx) >= 2) {
					// 判断是否唯一将牌
					continue;
				}
				vec_index.push_back(idx);
			}
			// 检查是否有单牌可出
			if (!vec_index.empty()) {
				for (auto temp : vec_index) {
					*out_card = SRAnalysis::switchToCardData(temp);
					out_card++;
					*out_card_count += 1;
				}
			}
		}


		// 拆掉 1233 1223  7789 这样的将牌,成为无将可听型
		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getShunDopant(idx)) != -1; ++idx) {
				vec_index.push_back(idx);
			}
			// 检查是否有牌可出
			if (!vec_index.empty()) {
				for (auto temp : vec_index) {
					*out_card = SRAnalysis::switchToCardData(temp);
					out_card++;
					*out_card_count += 1;
				}
			}

		}

		{
			vec_index.clear();
			for (int idx = 0; (idx = pmj->getDanDui(idx)) != -1; ++idx) {
				vec_index.push_back(idx);
			}
			// 检查是否有牌可出
			if (!vec_index.empty()) {
				int temp_index = vec_index.at(0);

				for (auto v_index : vec_index) {
					

					*out_card = SRAnalysis::switchToCardData(v_index);
					out_card++;
					*out_card_count += 1;

				}


				break;
			}
		}

	} while (false);


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

	int have_card_count = pmj->have(card);
	// 检验碰杠（四川麻将只有碰杠）
	if (have_card_count == 3) {
		// 杠牌逻辑分析
		pmj->delCard(card);
		pmj->delCard(card);
		pmj->delCard(card);
		return WIK_GANG;
	}
	else if (have_card_count == 2) {
		// 碰牌逻辑分析  
		pmj->delCard(card);
		pmj->delCard(card);
		return WIK_PENG;
	}
	
	return WIK_NULL;
}

void SRRobot::reset(void) {

	// 牌池中的麻将
	if (visibleMahjongPool_ != nullptr)
		delete visibleMahjongPool_;
	visibleMahjongPool_ = nullptr;
	// 牌墙上的麻将				 
	if (invisibleMahjongPool_ != nullptr)
		delete invisibleMahjongPool_;
	invisibleMahjongPool_ = nullptr;

	// 所有方位的麻将
	for (int i = 0; i < sizeof(mahjong_) / sizeof(mahjong_[0]); ++i) {
		if (mahjong_[i] != nullptr)
			delete mahjong_[i];
		mahjong_[i] = nullptr;
	}

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

SRMahjong * SRRobot::getMahjong(void) {
	return mahjong_[direction_];
}

const SRMahjong * SRRobot::getMahjong(enDirection drc) const {
	return mahjong_[drc];
}

void SRRobot::setMahjong(enDirection drc, SRMahjong * mahjong) {
	mahjong_[drc] = mahjong;
}

