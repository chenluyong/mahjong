// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SRMJROBOT_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SRMJROBOT_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifndef _SHANREN_ROBOT_H_
#define _SHANREN_ROBOT_H_
#ifdef SRMJROBOT_EXPORTS
#define SRMJROBOT_API __declspec(dllexport)
#else
#define SRMJROBOT_API __declspec(dllimport)
#endif

#include <iostream>

class SRMahjong;
class SRMahjongPool;

class SRMJROBOT_API SRRobot {

public:

	enum enDirection {
		None = -1,
		South = 0,              //南向 下
		West,					//西向 左
		North,                  //北向 上
		East,					//东向 右
	};

	SRRobot(void);
	virtual ~SRRobot(void);

public:

	// 摸牌
	// direction : 摸牌的方向，前摸牌:0，后摸牌:1。  默认为0
	virtual int touchCard(int direction = 0);
	
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
	void reset(void);

	// 设置当前机器人的方位
	void setDirection(enDirection drc);
	enDirection getDirection(void) const;

	// 设置牌局
	void setVisibleMahjongPool(SRMahjongPool* pool);
	void setInvisibleMahjongPool(SRMahjongPool* pool);

	// 设置麻将										  
	const SRMahjong* getMahjong(void) const;
	const SRMahjong* getMahjong(enDirection drc) const;
	void setMahjong(enDirection drc, SRMahjong* mahjong);

private:

//#ifdef SRMJROBOT_EXPORTS
private:
	// 主角方位
	enDirection direction_;

	// 牌池中的麻将
	SRMahjongPool* visibleMahjongPool_;
	// 牌墙上的麻将
	SRMahjongPool* invisibleMahjongPool_;

	// 所有方位的麻将
	SRMahjong* mahjong_[5];

	// 牌型是否可听
	int isTing_;
	// 听的牌
	unsigned char arrTing_[14];
	// 可听的数量
	unsigned char numTing_;

//#endif
};



#endif // _SHANREN_ROBOT_H_