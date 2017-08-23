#include "srmahjongtablewidget.h"
#include "srmahjonghallwidget.h"
#include "srmahjongseatwidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QGridLayout>
SRMahjongTableWidget::SRMahjongTableWidget(QWidget *parent) : QWidget(parent)
{
    init();
}

void SRMahjongTableWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,
                           &opt, &p, this);
}


void SRMahjongTableWidget::init(void)
{
    QGridLayout* layout = new QGridLayout();

    hallWidget_ = new SRMahjongHallWidget;
    for (int i =0; i < sizeof(seatWidget_)/ sizeof(seatWidget_[0]); ++i)
        seatWidget_[i] = new SRMahjongSeatWidget;

    seatWidget_[0]->setDirection(SRMahjongSeatWidget::enDirection::North);
    layout->addWidget(seatWidget_[0],0,1,1,8);  // ÉÏ
    seatWidget_[1]->setDirection(SRMahjongSeatWidget::enDirection::West);
    layout->addWidget(seatWidget_[1],1,0,8,1);  // ×ó
    layout->addWidget(hallWidget_,1,1,8,8);     // ÖÐ
    seatWidget_[2]->setDirection(SRMahjongSeatWidget::enDirection::East);
    layout->addWidget(seatWidget_[2],1,9,8,1);  // ÓÒ
    seatWidget_[3]->setDirection(SRMahjongSeatWidget::enDirection::South);
    layout->addWidget(seatWidget_[3],9,1,1,8);  // ÏÂ

    setLayout(layout);
}
