#ifndef SRPLAYERMODEL_H
#define SRPLAYERMODEL_H

#include "stdafx.h"

#include <QObject>

class SRPlayerModel : public QObject
{
    Q_OBJECT
public:
    explicit SRPlayerModel(QObject *parent = nullptr);

    void setDirection(enDirection _direction) { direction_ = _direction; }
    enDirection getDirection(void) {return direction_;}
public:

    enDirection direction_;

};

#endif // SRPLAYERMODEL_H
