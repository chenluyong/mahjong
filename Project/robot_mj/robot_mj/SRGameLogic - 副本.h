#ifndef _SHANREN_GAME_LOGIC_H_
#define _SHANREN_GAME_LOGIC_H_

#include "CMD_Sparrow.h"


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



//类型子项
struct tagKindItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbCardIndex[3];						//扑克索引
};

//组合子项
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户
};

//胡牌结果
struct tagChiHuResult
{
	DWORD							dwChiHuKind;						//吃胡类型
	DWORD							dwChiHuRight;						//胡牌权位
	DWORD							dwWinTimes;							//番数数目
};

//杠牌结果
struct tagGangCardResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[4];						//扑克数据
	BYTE                            cbGangType;							//杠牌类型
};

//分析子项
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//牌眼扑克
	BYTE							cbWeaveKind[MAX_WEAVE];						//组合类型
	BYTE							cbCenterCard[MAX_WEAVE];					//中心扑克
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//逻辑掩码

#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//////////////////////////////////////////////////////////////////////////
//动作定义

//动作标志
#define WIK_NULL					0x00								//没有类型
#define WIK_LEFT					0x01								//左吃类型
#define WIK_CENTER					0x02								//中吃类型
#define WIK_RIGHT					0x04								//右吃类型
#define WIK_PENG					0x08								//碰牌类型
#define WIK_GANG					0x10								//杠牌类型
#define WIK_LISTEN					0x20								//听牌类型
#define WIK_CHI_HU					0x40								//吃胡类型

//////////////////////////////////////////////////////////////////////////
//胡牌定义

//牌型掩码
#define CHK_MASK_SMALL				0x0000FFFF							//小胡掩码
#define CHK_MASK_GREAT				0xFFFF0000							//大胡掩码

//小胡牌型
#define CHK_NULL					0x00000000							//非胡类型
#define CHK_JI_HU					0x00000001							//鸡胡类型
#define CHK_PING_HU					0x00000002							//平胡类型

//大胡牌型
#define CHK_PENG_PENG				0x00010000							//碰碰胡牌
#define CHK_QI_XIAO_DUI				0x00020000							//七小对牌
#define CHK_SHI_SAN_YAO				0x00040000							//十三幺牌
#define CHK_YING_PAI				0x00080000							//硬牌
#define CHK_SAN_GODS				0x00100000							//三财神
#define CHK_SINGLE_PAI              0x00200000                          //单张胡牌
#define CHK_BA_DUI                  0x00400000                          //八对
#define CHK_YING_BA_DUI             0x00800000                          //硬八对


//////////////////////////////////////////////////////////////////////////
//胡牌权位

//牌权掩码
#define CHR_MASK_SMALL				0x000000FF							//小胡掩码
#define CHR_MASK_GREAT				0xFFFFFF00							//大胡掩码

//大胡权位
#define CHR_DI						0x00000100							//地胡权位
#define CHR_TIAN					0x00000200							//天胡权位
#define CHR_QING_YI_SE				0x00000400							//清一色牌
#define CHR_QIANG_GANG				0x00000800							//抢杆权位
#define CHK_QUAN_QIU_REN			0x00001000							//全求权位
//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class SRGameLogic
{
	//变量定义
protected:
	static const BYTE				cbCardDataArray_[MAX_REPERTORY];	//扑克数据
	BYTE							bGodsCardData_;          // 财神

									
						//函数定义
public:
	//构造函数
	SRGameLogic();
	//析构函数
	virtual ~SRGameLogic();

	//控制函数
public:
	//混乱扑克
	void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//删除扑克
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbRemoveCard[], BYTE cbRemoveCount);

	//辅助函数
public:
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//扑克数目
	BYTE GetCardCount(BYTE cbCardIndex[MAX_INDEX]);
	//组合扑克
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

	//等级函数
public:
	//动作等级
	BYTE GetUserActionRank(BYTE cbUserAction);
	//胡牌等级
	WORD GetChiHuActionRank(tagChiHuResult & ChiHuResult);

	//动作判断
public:
	//吃牌判断
	BYTE EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//碰牌判断
	BYTE EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//杠牌判断
	BYTE EstimateGangCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

	//动作判断
public:
	//听牌分析
	BYTE AnalyseTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, DWORD dwChiHuRight);
	//杠牌分析
	BYTE AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, tagGangCardResult & GangCardResult);
	//吃胡分析
	BYTE AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE cbCurrentCard, DWORD dwChiHuRight, tagChiHuResult & ChiHuResult);

	//特殊胡牌
public:
	//清一色牌
	bool IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//七小对牌
	bool IsQiXiaoDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//十三夭牌
	bool IsShiSanYao(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount);

	// 八对
	bool IsBaDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount);

	//转换函数
public:
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

	void SetGodsCard(BYTE byCardData);

	//内部函数
private:
	//分析扑克
	//bool AnalyseCard(BYTE cbCardIndexUser[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
};


#endif // _SHANREN_GAME_LOGIC_H_