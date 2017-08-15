#include "srcommon.h"

QString sr::convertCard(BYTE card)
{

   BYTE card_type = card & 0xF0;

   BYTE card_num = card & 0x0F;

   QString name;

   switch (card_type) {
   case 0x00:
       name = QStringLiteral(" 万");
       break;
   case 0x10:
       name = QStringLiteral(" 条");
       break;
   case 0x20:
       name = QStringLiteral(" 同");
       break;
   case 0x30:
       name = QStringLiteral(" 风");
       break;
   default:
       break;
   }

   name = QString::number(card_num) + name;

   return name;
}


const char* const sr::getDirectionNameC(enDirection direction)
{
    switch (direction) {
    case ::enDirection::South:
        return "位南";
    case ::enDirection::West:
        return "位西";
    case ::enDirection::North:
        return "位北";
    case ::enDirection::East:
        return "位东";
    default:
        return "????";
    }
}

QString sr::getDirectionNameQ(enDirection direction)
{
    switch (direction) {
    case ::enDirection::South:
        return QStringLiteral("位南");
    case ::enDirection::West:
        return QStringLiteral("位西");
    case ::enDirection::North:
        return QStringLiteral("位北");
    case ::enDirection::East:
        return QStringLiteral("位东");
    default:
        return QStringLiteral("????");
    }
}
