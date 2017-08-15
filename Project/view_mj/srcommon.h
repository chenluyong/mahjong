#ifndef SRCOMMON_H
#define SRCOMMON_H

#include "stdafx.h"
#include <QString>

namespace sr {

QString convertCard(BYTE card);

QString getDirectionNameQ(enDirection direction);
const char* const getDirectionNameC(enDirection direction);
}




#endif // SRCOMMON_H
