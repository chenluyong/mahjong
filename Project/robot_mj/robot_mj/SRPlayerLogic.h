
#ifndef _SHANREN_PLAYER_LOGIC_H_
#define _SHANREN_PLAYER_LOGIC_H_

#include "CMD_Sparrow.h"
#include <vector>

class SRPlayerLogic
{
public:


public:
	SRPlayerLogic(enDirection dir);
	virtual ~SRPlayerLogic();


	// 获得一张不要的麻将牌
	int getDiscard(void);



private:
	// 座位方向
	enDirection direction_;
	// 当前麻将
	std::vector<BYTE> vecCardData_;

};


#endif // _SHANREN_PLAYER_LOGIC_H_