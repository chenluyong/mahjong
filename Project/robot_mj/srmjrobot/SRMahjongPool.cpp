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

unsigned char SRMahjongPool::find(unsigned char card_data) const
{
	const unsigned char& rcount = indexEnd_ - indexBegin_;

	for (unsigned char i = 0; i < rcount; ++i)
		if (cardData_[i + indexBegin_] == card_data)
			return i;

	return -1;
}

unsigned char SRMahjongPool::have(unsigned char card_data) const
{
	unsigned char num = 0;
	const int& rcount = indexEnd_ - indexBegin_;

	for (int i = 0; i < rcount; ++i)
		if (cardData_[i + indexBegin_] == card_data)
			++num;

	return num;
}









///////////////////////SRInvisibleMahjongPool////////////////////////





unsigned char SRInvisibleMahjongPool::pop_front(void)
{
	return cardData_[indexBegin_++];
}

unsigned char SRInvisibleMahjongPool::pop_back(void)
{
	return cardData_[indexEnd_--];
}

int SRInvisibleMahjongPool::setMahjongPoolData(const unsigned char * _data, unsigned char size)
{
	if (_data == nullptr)
		return -87;


	const int& temp_size = sizeof(cardData_);
	memset(cardData_, 0, temp_size);
	int ret = 0;

	if (size > temp_size) {
		size = temp_size;
		ret = -87;
	}

	memcpy(cardData_, _data, size);

	return ret;
}




///////////////////////SRVisibleMahjongPool////////////////////////





void SRVisibleMahjongPool::push_back(unsigned char data)
{
	cardData_[indexBegin_++] = data;
}
