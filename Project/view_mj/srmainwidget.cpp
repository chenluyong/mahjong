#include "srmainwidget.h"
#include "srplayerdesktop.h"
#include "srdesktop.h"

#include <QDebug>
#include <QGridLayout>


using namespace std;


SRMainWidget::SRMainWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(1000,1000);

    initServe();

    // 洗牌
    desktop_->onShuffle();

    connect(desktop_, SIGNAL(emDealCard(enDirection,BYTE*,BYTE)),
            player_[0],SLOT(onDealCard(enDirection,BYTE*,BYTE)));
    connect(desktop_, SIGNAL(emDealCard(enDirection,BYTE*,BYTE)),
            player_[1],SLOT(onDealCard(enDirection,BYTE*,BYTE)));
    connect(desktop_, SIGNAL(emDealCard(enDirection,BYTE*,BYTE)),
            player_[2],SLOT(onDealCard(enDirection,BYTE*,BYTE)));
    connect(desktop_, SIGNAL(emDealCard(enDirection,BYTE*,BYTE)),
            player_[3],SLOT(onDealCard(enDirection,BYTE*,BYTE)));


    connect(player_[0], SIGNAL(emOutCard(enDirection,BYTE)),
            desktop_,SIGNAL(emPlayCard(enDirection,BYTE)));
    connect(player_[1], SIGNAL(emOutCard(enDirection,BYTE)),
            desktop_,SIGNAL(emPlayCard(enDirection,BYTE)));
    connect(player_[2], SIGNAL(emOutCard(enDirection,BYTE)),
            desktop_,SIGNAL(emPlayCard(enDirection,BYTE)));
    connect(player_[3], SIGNAL(emOutCard(enDirection,BYTE)),
            desktop_,SIGNAL(emPlayCard(enDirection,BYTE)));


    connect(desktop_, SIGNAL(emPlayCard(enDirection,BYTE)),
            player_[0],SLOT(onOtherPlayState(enDirection,BYTE)));
    connect(desktop_, SIGNAL(emPlayCard(enDirection,BYTE)),
            player_[1],SLOT(onOtherPlayState(enDirection,BYTE)));
    connect(desktop_, SIGNAL(emPlayCard(enDirection,BYTE)),
            player_[2],SLOT(onOtherPlayState(enDirection,BYTE)));
    connect(desktop_, SIGNAL(emPlayCard(enDirection,BYTE)),
            player_[3],SLOT(onOtherPlayState(enDirection,BYTE)));





    // 发牌
    for (int i = 0; i < GAME_PLAYER; ++i)
        desktop_->onDealCard(player_[i]->getDirection(), 13);


    // 开局
    desktop_->onStart();

}

SRMainWidget::~SRMainWidget()
{

}

void SRMainWidget::initServe()
{
    player_[0] = new SRPlayerDesktop(this);
    player_[1] = new SRPlayerDesktop(this);
    player_[2] = new SRPlayerDesktop(this);
    player_[3] = new SRPlayerDesktop(this);
    desktop_ = new SRDesktop(this);

    // 麻将桌布局
    QGridLayout* layout = new QGridLayout;
//    layout->setMargin(0);


    player_[0]->setDirection(enDirection::North);
    player_[1]->setDirection(enDirection::West);
    player_[2]->setDirection(enDirection::East);
    player_[3]->setDirection(enDirection::South);

    layout->addWidget(player_[0],0,1,1,8); // 上
    layout->addWidget(player_[1],1,0,8,1); // 左
    layout->addWidget(desktop_,1,1,8,8); // 中
    layout->addWidget(player_[2],1,9,8,1); // 右
    layout->addWidget(player_[3],9,1,1,8); // 下

    setLayout(layout);
}
