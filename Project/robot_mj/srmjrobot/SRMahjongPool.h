#ifndef _SHANREN_MAHJONG_POOL_H_
#define _SHANREN_MAHJONG_POOL_H_


#ifdef SRMJROBOT_EXPORTS
#define SRMJROBOT_API __declspec(dllexport)
#else
#define SRMJROBOT_API __declspec(dllimport)
#endif

#include "SRMahjong.h"


class SRMJROBOT_API SRMahjongPool :
	public SRMahjong
{
public:

	virtual ~SRMahjongPool(void);


public:
	void touchCard(SRMahjong* object, int direction);

	

};

#endif // _SHANREN_MAHJONG_POOL_H_