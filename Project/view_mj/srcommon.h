#ifndef SRCOMMON_H
#define SRCOMMON_H

#include "stdafx.h"
#include <QString>

namespace sr {

QString convertCard(BYTE card);
//BYTE convertCard(QStrisng card);

QString getDirectionNameQ(enDirection direction);
const char* const getDirectionNameC(enDirection direction);
}




#endif // SRCOMMON_H
