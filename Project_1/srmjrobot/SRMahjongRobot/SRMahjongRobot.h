
#include "SRMahjongGlobal.h"

class SRMahjong;
class SRMahjongAnalysis;
class SRVisibleMahjongPool;
class SRInvisibleMahjongPool;

// 此类是从 SRMahjongRobot.dll 导出的
class SRMAHJONGROBOT_API SRMahjongRobot {

public:

	SRMahjongRobot(void);

public:

	// 摸牌
	// card : 摸到的牌
	virtual int touchCard(unsigned char card);
	
	// 出牌
	// out_card : 出牌的数据
	// out_card_count : 出牌的张数
	virtual int getOutCard(unsigned char* out_card, unsigned char* out_card_count);
	
	// 询问动作
	// card : 是其他牌桌打出的牌
	// 返回值为具体动作，详见头文件的返回值定义
	virtual int getAction(enDirection drc, unsigned char card);
	

public:
	// 重置游戏
//	void reset(void);
	
	// 设置当前机器人的方位
	inline void setDirection(enDirection drc);
	inline enDirection getDirection(void) const { return direction_; }
	
	// 设置牌局
	inline void setVisibleMahjongPool(SRVisibleMahjongPool* pool) { visibleMahjongPool_ = pool; }
	inline void setInvisibleMahjongPool(SRInvisibleMahjongPool* pool) { invisibleMahjongPool_ = pool; }
	
	// 设置麻将										  
	inline SRMahjong* getMahjong(void);
	inline const SRMahjong* getMahjong(enDirection drc) const;
	inline void setMahjong(enDirection drc, SRMahjong* mahjong);

	// 设置需要针对的敌对玩家
	inline void setEnemy(enDirection drc, int isEnemy = 1);
	
private:

	// 主角方位
	enDirection direction_;

	// 敌对玩家
	int enemyDirection_[4];

	// 牌局轮次
	int roundNumber_;

	// 牌池中的麻将
	SRVisibleMahjongPool* visibleMahjongPool_;
	// 牌墙上的麻将
	SRInvisibleMahjongPool* invisibleMahjongPool_;

	SRMahjongAnalysis* analysisLogic_;

	// 所有方位的麻将
	SRMahjong* mahjong_[5];




//	// 摸牌
//	// direction : 摸牌的方向，前摸牌:0，后摸牌:1。  默认为0
//	virtual int touchCard(unsigned char card);
//
//	// 出牌
//	// out_card : 出牌的数据
//	// out_card_count : 出牌的张数
//	virtual int getOutCard(unsigned char* out_card, unsigned char* out_card_count);
//
//	// 询问动作
//	// card : 是其他牌桌打出的牌
//	// 返回值为具体动作，详见头文件的返回值定义
//	virtual int getAction(enDirection drc, unsigned char card);
//
//
//public:
//	// 重置游戏
//	void reset(void);
//
//	// 设置当前机器人的方位
//	void setDirection(enDirection drc);
//	enDirection getDirection(void) const;
//
//	// 设置牌局
//	void setVisibleMahjongPool(SRVisibleMahjongPool* pool);
//	void setInvisibleMahjongPool(SRInvisibleMahjongPool* pool);
//
//	// 设置麻将										  
//	SRMahjong* getMahjong(void);
//	const SRMahjong* getMahjong(enDirection drc) const;
//	void setMahjong(enDirection drc, SRMahjong* mahjong);
//
//private:
//
//	//#ifdef SRMJROBOT_EXPORTS
//private:
//	// 主角方位
//	enDirection direction_;
//
//	// 牌池中的麻将
//	SRVisibleMahjongPool* visibleMahjongPool_;
//	// 牌墙上的麻将
//	SRInvisibleMahjongPool* invisibleMahjongPool_;
//
//	// 所有方位的麻将
//	SRMahjong* mahjong_[5];
//
//	// 牌局轮次
//	int roundNumber_;
//
//	// 牌型是否可听
//	int isTing_;
//	// 听的牌
//	unsigned char arrTing_[14];
//	// 可听的数量
//	unsigned char numTing_;
//
//	//#endif
};

