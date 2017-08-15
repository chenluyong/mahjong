#ifndef SRCARDMODEL_H
#define SRCARDMODEL_H

#include "stdafx.h"
#include <functional>
#include <QObject>

class SRCardModel : public QObject
{
    Q_OBJECT
public:
    explicit SRCardModel(QObject *parent = nullptr);

    void setCardData(BYTE* _cardData, BYTE _cardCount = MAX_COUNT);

    BYTE getCardData(int _index);



signals:


signals:

public slots:


private:
    BYTE cardData_[MAX_COUNT + 1];
    BYTE cardCount_ = MAX_COUNT;

};

#endif // SRCARDMODEL_H
