#pragma once

#include "SRMahjongGlobal.h"



class SRMAHJONGROBOT_API SRMahjong {

public:

	SRMahjong(void);

	virtual ~SRMahjong(void);

public:

	// 获得手牌数据
	inline const BYTE* getData(void) const { return cardData_; }

	// 设置手牌数据
	void setCardData(const BYTE * _cardData, BYTE _cardCount);

	// 通过下标访问索引手牌
	inline BYTE index(int idx) const { return cardIndex_[idx]; }
	// 通过下标访问索引手牌
	inline const BYTE* getIndex(int idx) const { return &cardIndex_[idx]; }
						
	// 查询某张牌有多少张
	int have(BYTE _card);

	// 删除一张手牌
	int removeCardData(BYTE _card);

	// 添加一张手牌
	int addCardData(BYTE _card);

	// 当前手牌数量
	inline int count(void) const;

	// 获得最后一张添加的牌
	inline BYTE getLastTouchCard(void) const { return lastTouchCard_; }

public:

	static unsigned char switchToCardIndex(unsigned char cbCardData) {
		return ((cbCardData&MASK_COLOR) >> 4) * 9 + (cbCardData&MASK_VALUE) - 1;
	}

	static unsigned char switchToCardData(unsigned char cbCardIndex) {
		return ((cbCardIndex / 9) << 4) | (cbCardIndex % 9 + 1);
	}

protected:

	// 更新麻将牌的索引数据
	void upCardIndex(void);

	inline int maxSize(void) const;

protected:

	BYTE cardData_[MAX_COUNT];

	BYTE cardIndex_[MAX_INDEX];

private:

	// 最后一张摸的牌
	unsigned char lastTouchCard_;

};


class SRMAHJONGROBOT_API SRMahjongCard : public SRMahjong {

public:
	SRMahjongCard(void);

	// 获得一张孤立的番牌
	int getFanPaiOne(int _indexBegin = 0) const;

	// 获得间隔1个空位的不连续牌，从两头向中间排查
	int getIntervalOne(int _indexBegin = 0) const;

	// 获得间隔2个空位的不连续牌，从两头向中间排查
	int getIntervalTwo(int _indexBegin = 0) const;

	// 1123  5667 5677
	int getShunDopant(int _indexBegin = 0) const;

	// 112  899  233
	int getDanDui(int _indexBegin = 0) const;

	int getNotShunKe(int _indexBegin = 0) const;

	int analysis(void);

protected:

	int analysisHuaPai(const BYTE * cardIndexBegin, 
		const BYTE * cardIndexEnd, BYTE * outCardIndexState)const;



private:
	// 0x00 无类型 0x01 对子类型 0x02 刻类型 0x04 杠类型 0x08 顺子类型  
	BYTE cardIndexState_[MAX_INDEX];

};