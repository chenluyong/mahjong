#ifndef _SHANREN_INTERFACE_BASE_H_
#define _SHANREN_INTERFACE_BASE_H_

#include "CMD_Sparrow.h"


class SRCardBase {
	SRCardBase(void) {};

	virtual ~SRCardBase(void) {};

	virtual BYTE* data(void) { return cardData_; }

	virtual void setData(BYTE* _data, int size);


private:
	BYTE* cardData_;
	int size_;
};

class SRInterfaceBase
{
public:

	SRInterfaceBase()
	{
	}

	virtual ~SRInterfaceBase()
	{
	}



	// 获得桌面的麻将数据
	virtual int getDesktopCardData(SRCardBase* _outCard) = 0;
														
	// 获得墙上所有的麻将数据 (即：待摸的牌)
	virtual int getWallCardData(SRCardBase* _outCard) = 0;

	// 获得指定玩家的麻将数据
	virtual int getPlayerCardData(enDirection _drc, SRCardBase* _outCard) = 0;


	// 获得当前游戏回合数
	virtual int getBout(int* _outBout) = 0;
};

#endif // _SHANREN_INTERFACE_BASE_H_