#ifndef _SHANREN_MAHJONG_POOL_H_
#define _SHANREN_MAHJONG_POOL_H_

#include "srmjglobal.h"

#ifdef SRMJROBOT_EXPORTS
#define SRMJROBOT_API __declspec(dllexport)
#else
#define SRMJROBOT_API __declspec(dllimport)
#endif



class SRMJROBOT_API SRMahjongPool
{
public:

	virtual ~SRMahjongPool(void);


public:

	int getMahjongPoolData(unsigned char* out_data, unsigned char size);

	int find(unsigned char card_data)const;

	unsigned char pop_front(void);
	unsigned char pop_back(void);


private:

	unsigned char cardData_[MAX_COUNT + 1];

	unsigned char indexBegin_;
	unsigned char indexEnd_;


};

#endif // _SHANREN_MAHJONG_POOL_H_