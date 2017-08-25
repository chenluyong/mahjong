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

    direction_ = enDirection::None;
    mahjong_ = nullptr;
    robot_ = nullptr;

    for(int i = 0; i < 14; ++i) {
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
}

void SRMahjongSeatWidget::setMahjong(SRMahjong *mahjong)
{
    mahjong_ = mahjong;
    if (robot_ != nullptr)
        robot_->setMahjong(direction_, mahjong_);

    upMahjongBox();
}

void SRMahjongSeatWidget::setDirection(enDirection drc)
{
    if (direction_ != enDirection::None)
        qDebug() << __FUNCTION__ << "double set seat direction";

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

void SRMahjongSeatWidget::onPlayerOutCard(unsigned char data)
{
    int ret_action = robot_->getAction(direction_,data);

    if (ret_action == WIK_GANG || ret_action == WIK_PENG) {
        emit sigAction(direction_,ret_action,data);
    }
    else if (ret_action == WIK_CHI_HU) {
        emit sigHu(direction_);
    }

    return;

}

void SRMahjongSeatWidget::touchCard(unsigned char data)
{
    robot_->touchCard(data);

    unsigned char out_card[MAX_COUNT] = {};
    unsigned char card_size = 0;
    int ret_action = robot_->getOutCard(out_card,&card_size);

    if (ret_action == WIK_NULL) {
        emit sigOutCard(direction_,out_card[0]);
    }
    else if (ret_action == WIK_CHI_HU) {
        emit sigHu(direction_);
        setStyleSheet("QWidget{background-color:red;}" \
                      "QWidget:hover{border: 1px solid green;}");
        return;
    }
    else {
        emit sigAction(direction_,ret_action,out_card[0]);
    }

    // 删除计划打出的牌
    for (auto item : listMahjong_) {
        if (card_size == 0)
            break;
        if (item->getCardData() == out_card[0]) {
            item->onModifyData(0);
            --card_size;
        }
    }

    upMahjongBox();

}

void SRMahjongSeatWidget::onMahjongKnockout(QWidget *object)
{
    SRMahjongWidget * temp_object = qobject_cast<SRMahjongWidget*>(object);

    if (temp_object == nullptr) {
        qDebug() << __FUNCTION__ << "convert widget error.";
        return ;
    }

    // 打出这张牌
    emit sigOutCard(direction_,temp_object->getCardData());
    temp_object->onModifyData(0);

    upMahjongBox();

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

void SRMahjongSeatWidget::upMahjongBox()
{
    if (mahjong_ == nullptr) {
        qDebug() << "error: mahjong is null!";
        return;
    }
    mahjong_->sort();
    const unsigned char* pmj_data = mahjong_->data();

    for (auto item : listMahjong_) {
        item->onModifyData(*pmj_data++);
    }


}


