#ifndef _SHANREN_GAME_LOGIC_H_
#define _SHANREN_GAME_LOGIC_H_

#include "CMD_Sparrow.h"
#include "SRPlayerLogic.h"
#include "SRInterfaceBase.h"

#include <vector>


// *  胡牌类型  * //

// 平胡：四坎牌，加一对将

// 对对胡：每坎牌都是三张一样的牌，即111万、222条

// 抢杠胡：它人杠的时候，抢牌胡

// 清一色：全手牌是一种花色，即123条、567条、234 条、888条、99条

// 幺九牌；每坎牌都是一或九

// 七对；胡牌的时候是七对牌，即11万、22万、99万、44筒、66筒、88筒、99筒

// 清七对；在七对的基础上，有两对牌是四张一样的。（注意:此四张牌并不是杠的牌） 即11万、11万、99万、44筒、66筒、88筒、99筒

// 清对；一种花色的大对子。即111万、444万、222万、999万、66万，得四分。

// 将对；即二、五、八的大对子，即222万 555万 888万 222条 55筒，的四分。

// 天胡；即刚码好牌就自然成胡，得8分。

// 地胡；即刚打第一张牌就成胡，得8分。

		   


//游戏逻辑类
class SRGameLogic
{
						//函数定义
public:
	
	/*
		构造函数				
		_gameId		: 游戏ID
		_ruleId		: 游戏规则ID
		_playerNum	: 游戏人数
		_character	: 机器人性格或难度
	*/
	SRGameLogic(int _gameId = 0, int _ruleId = 0, int _playerNum = 4, int character = 0);

	//析构函数
	virtual ~SRGameLogic(void);

public:

	// 询问出牌														 
	int askChuCard(BYTE* _card);

	// 询问碰牌														 
	int askPengCard(BYTE* _card);

	// 询问杠牌														 
	int askGangCard(BYTE* _card);

	// 询问吃牌														 
	int askChiCard(BYTE _inCard1, BYTE _inCard2, BYTE* _outCard);

	// 询问胡牌														 
	int askHuCard(void);
	  





	// 游戏逻辑

public:



	// 游戏基础环境信息配备
private:
	/*
	安装游戏场景	  	
	_gameId		: 游戏ID
	_ruleId		: 游戏规则ID
	_playerNum	: 游戏人数
	_character	: 机器人性格或难度
	*/
	void installGameScene(int _gameId, int _ruleId, int _playerNum, int character);


private:

	// 玩家资源类
	std::vector<SRPlayerLogic*> vecPlayer_;


};


#endif // _SHANREN_GAME_LOGIC_H_