#ifndef _SHANREN_MAHJONG_H_
#define _SHANREN_MAHJONG_H_

#ifdef SRMJROBOT_EXPORTS
#define SRMJROBOT_API __declspec(dllexport)
#else
#define SRMJROBOT_API __declspec(dllimport)
#endif

#include "srmjglobal.h"

class SRMJROBOT_API SRMahjong
{
public:
	SRMahjong(unsigned char cardData[], unsigned char cardCount);
	virtual ~SRMahjong();

public:
	
	void sort(void);

	const unsigned char* data(void) const { return cardData_; }

	unsigned char size(void) const;


public:

	unsigned char operator[] (const int &index) { return cardIndex_[index]; }
	unsigned char getIndex(unsigned char& index) { return cardIndex_[index]; }
	 
public:

	// 获得一张孤立的番牌
	int getFanPaiOne(int _indexBegin) const;

	// 获得间隔1个空位的不连续单牌，从两头向中间排查
	int getIntervalOne(int _indexBegin = 0) const;

	// 获得间隔2个空位的不连续单牌，从两头向中间排查
	int getIntervalTwo(int _indexBegin = 0) const;


protected:


private:

	// 麻将的索引数据
	unsigned char cardIndex_[MAX_INDEX];

	unsigned char cardData_[MAX_REPERTORY + 1];
	unsigned char cardCount_;
	unsigned char index_;

};

#endif // _SHANREN_MAHJONG_H_