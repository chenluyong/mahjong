#include "SRMahjongPool.h"

#include <iostream>




SRMahjongPool::~SRMahjongPool()
{
	indexBegin_ = 0;
	indexEnd_ = sizeof(cardData_) / sizeof(cardData_[0]);
}

int SRMahjongPool::getMahjongPoolData(unsigned char * out_data, unsigned char size)
{
	if (out_data == nullptr)
		return -87;

	const int& temp_size = indexEnd_ - indexBegin_;

	if (size > temp_size && temp_size > 0)
		size = temp_size;

	memcpy(out_data, &cardData_[indexBegin_], size);

	return 0;
}

int SRMahjongPool::find(unsigned char card_data) const
{
	const int& rcount = indexEnd_ - indexBegin_;

	for (int i = 0; i < rcount; ++i)
		if (cardData_[i + indexBegin_] == card_data)
			return i;

	return -1;
}

unsigned char SRMahjongPool::pop_front(void)
{
	return cardData_[indexBegin_++];
}

unsigned char SRMahjongPool::pop_back(void)
{
	return cardData_[indexEnd_--];
}
