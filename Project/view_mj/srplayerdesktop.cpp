#include "srplayerdesktop.h"
#include "srplayermodel.h"
#include "srcardmodel.h"

#include <iomanip>
#include <iostream>
#include <algorithm>

#include <QDebug>
#include <QPushButton>


SRPlayerDesktop::SRPlayerDesktop(QWidget *parent) : QWidget(parent)
{

    model_ = new SRPlayerModel;
    layout_ = new QBoxLayout(QBoxLayout::LeftToRight);
    cardModel_ = new SRCardModel;

    for(int i = 0; i < MAX_COUNT; ++i) {
        listButton_.push_back(new QPushButton());
        layout_->addWidget(listButton_.at(i));
        listButton_.at(i)->setFixedSize(50,75);
    }

    setLayout(layout_);
}

void SRPlayerDesktop::setDirection(enDirection _direction) {
     model_->setDirection(_direction);

     if(_direction == enDirection::South || _direction == enDirection::North) {
        layout_->setDirection(QBoxLayout::LeftToRight);
     }
     else if (_direction == enDirection::West ||_direction == enDirection::East) {
        layout_->setDirection(QBoxLayout::TopToBottom);
     }

}

enDirection SRPlayerDesktop::getDirection()
{
    return model_->getDirection();
}

void SRPlayerDesktop::onDealCard(enDirection direction, BYTE *data, BYTE count)
{
    if (direction != model_->getDirection())
        return;

    // 第一次分发
    if (count == MAX_COUNT)
        cardModel_->setCardData(data,count);

//    std::cout << getDirectionName() << ": "<< std::endl;
//    for (int i = 0; i < count; ++i)
//        std::cout << std::showbase << std::setw(5) << std::hex <<(int)data[i] << ",";
//    std::cout << std::endl;
}


const char* const SRPlayerDesktop::getDirectionName()
{
    switch (model_->getDirection()) {
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

