#include "srmahjongseatwidget.h"
#include "control/srmahjongwidget.h"

#include <QDebug>
#include <QBoxLayout>
#include <QStyleOption>
#include <QPainter>

#include <srmjrobot.h>
#include <SRMahjong.h>

SRMahjongSeatWidget::SRMahjongSeatWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("QWidget{background-color:green;}" \
                  "QWidget:hover{border: 1px solid red;}");

    for(int i = 0; i < 13; ++i) {
        SRMahjongWidget* temp = new SRMahjongWidget();
        listMahjong_.push_back(temp);

        connect(temp,SIGNAL(sigDoubleClick(QWidget*)),
                this,SLOT(onMahjongKnockout(QWidget*)));
    }
}

void SRMahjongSeatWidget::setRobot(SRRobot *robot)
{
    robot_ = robot;
    if (robot_ != nullptr) {
        robot_->setDirection(direction_);
    }
//    if (mahjong_ != nullptr && robot_ != nullptr) {
//    }
//        robot->setMahjong(direction_, mahjong_);
}

void SRMahjongSeatWidget::setMahjong(SRMahjong *mahjong)
{
    mahjong_ = mahjong;
    if (robot_ != nullptr)
        robot_->setMahjong(direction_, mahjong_);
}

void SRMahjongSeatWidget::setDirection(enDirection drc)
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

void SRMahjongSeatWidget::onTouchCard(unsigned char *data, unsigned char count)
{
    Q_UNUSED(count);
    robot_->touchCard(*data);
}

void SRMahjongSeatWidget::onMahjongKnockout(QWidget *object)
{
    SRMahjongWidget * temp_object = qobject_cast<SRMahjongWidget*>(object);

    if (temp_object == nullptr) {
        qDebug() << __FUNCTION__ << "convert widget error.";
        return ;
    }

    // 打出这张牌

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


