#ifndef _SHANREN_ANALYZE_CARD_H_
#define _SHANREN_ANALYZE_CARD_H_

#include "mj_header.h"
#include "SRCardMagic.h"
#include <map>
#include <vector>




class SRAnalyzeCard
{
public:
	SRAnalyzeCard();
	virtual ~SRAnalyzeCard();


	// 游戏基础资源设置
public:
	void reset(void);
	// 设置已打出的牌
	void setGameCardForShow(stCardData _data);

	// 设置未翻开的牌
	void setGameCardForHide(stCardData _data);

	// 设置玩家的牌
	void setGameCardForPlayer(enDirection _dec, stCardData _data);

	// 设置主人公玩家
	void setGameProtagonists(enDirection _dec);


	// 牌型分析
public:
	// 获得要出的牌
	int getOutCard(unsigned char _in_card, unsigned char* _out_card);

	int getAction(unsigned char _in_card);


private:
	// 分析玩家牌型
	void analyzeGamePlayer(enDirection _in_dec, stCardData* _out_dynamic, stCardData* _out_static);

	int analyzeProtagonists(void);


	// 检索出所有玩家所持顺子
	void analyzeGamePlayerForChain(void);


private:



private:


	bool isTing_ = false;

	std::vector<int> vecTingIndex_;

	// 已出的牌型
	stCardData showCardData_;
	// 未摸的牌型
	stCardData hideCardData_;
	// 玩家牌
	std::map<enDirection, SRCardMagic> mapCardData_;

    // 主人公所处方位
	enDirection protagonists_;


};


#endif // _SHANREN_ANALYZE_CARD_H_