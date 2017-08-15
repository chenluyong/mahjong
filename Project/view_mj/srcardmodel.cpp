#include "srcardmodel.h"

#include <QDebug>

SRCardModel::SRCardModel(QObject *parent) : QObject(parent)
{
    memset(cardData_,0,sizeof(cardCount_));
}

void SRCardModel::setCardData(BYTE *_cardData, BYTE _cardCount)
{
    if (_cardCount > MAX_COUNT)
        return;
    else if (_cardCount == MAX_COUNT) {
        memset(cardData_, 0, sizeof(cardData_));
        memcpy(cardData_, _cardData, _cardCount);
        cardCount_ = _cardCount;
    }


    std::sort(cardData_,cardData_+cardCount_);

}

BYTE SRCardModel::getCardData(int _index)
{
    if (_index > cardCount_)
        return -1;

    return cardData_[_index];
}
