#include "srmahjongseatwidget.h"
#include "control/srmahjongwidget.h"
#include <QDebug>
#include <QBoxLayout>
#include <QStyleOption>
#include <QPainter>
SRMahjongSeatWidget::SRMahjongSeatWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("QWidget{background-color:green;}" \
                  "QWidget:hover{border: 1px solid red;}");

    for(int i = 0; i < 13; ++i)
        listMahjong_.push_back(new SRMahjongWidget());

}

void SRMahjongSeatWidget::setDirection(SRMahjongSeatWidget::enDirection drc)
{
    direction_ = drc;
    QBoxLayout * layout;

    switch (drc) {
    case South:
        layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
        break;
    case West:
        layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
        break;
    case North:
        layout = new QBoxLayout(QBoxLayout::Direction::RightToLeft);
        break;
    case East:
        layout = new QBoxLayout(QBoxLayout::Direction::BottomToTop);
        break;
    default:
        qDebug() << __FUNCTION__ << "??? direction error.";
        break;
    }

    // 将麻将加入布局
    for (auto item : listMahjong_){
        layout->addWidget(item);
    }

    setLayout(layout);


}

void SRMahjongSeatWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);

    style()->drawPrimitive(QStyle::PE_Widget,
                           &opt, &p, this);

}


