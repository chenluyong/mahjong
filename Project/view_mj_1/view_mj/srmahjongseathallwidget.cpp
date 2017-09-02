#include "srmahjongseathallwidget.h"

#include <QBoxLayout>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>

#include "control/srmahjongwidget.h"


SRMahjongSeatHallWidget::SRMahjongSeatHallWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("QWidget{border-image:url(:/bg/back);}" \
                  "QWidget:hover{border: 1px solid red;}");
    layout_ = new QGridLayout;
    direction_ = enDirection::None;
    row_ = 0;
    col_ = 0;
}

void SRMahjongSeatHallWidget::push_back(SRMahjongWidget *mahjong) {
    if (layout_ == nullptr) {
        qDebug() << "error, not set layout.";
        return;
    }

    layout_->addWidget(mahjong, row_, col_, 1, 1);

    if (enDirection::South == direction_
     || enDirection::North == direction_) {
        if (++col_ == 8) {
            row_++;
            col_ = 0;
        }
    }
    else {
        if (++row_ == 8) {
            col_++;
            row_ = 0;
        }
    }

}



void SRMahjongSeatHallWidget::setDirection(enDirection drc) {
    if (direction_ != enDirection::None) {
        qDebug() << __FUNCTION__ << "double set seat direction";
        return;
    }



    direction_ = drc;

    switch (drc) {
    case South:
    case North:
        break;
    case West:
    case East:
        break;
    default:
        qDebug() << __FUNCTION__ << "??? direction error.";
        break;
    }

    // 将麻将加入布局
    for (auto item : listMahjong_){
        layout_->addWidget(item);
    }



    setLayout(layout_);

}
void SRMahjongSeatHallWidget::paintEvent(QPaintEvent *event)
{

    Q_UNUSED(event);

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);

    style()->drawPrimitive(QStyle::PE_Widget,
                           &opt, &p, this);
}
