#include "srmahjongseatwidget.h"
#include "control/srmahjongwidget.h"
#include "srmahjonghallwidget.h"
#include "srmahjongseathallwidget.h"

#include <QDebug>
#include <QBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>

#include <srmjrobot.h>
#include <SRMahjong.h>
#include <SRMahjongPool.h>

SRMahjongSeatWidget::SRMahjongSeatWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("QWidget{background-color:green;}" \
                  "QWidget:hover{border: 1px solid red;}");

    direction_ = enDirection::None;

    hallWidget_ = nullptr;
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
    if (direction_ != enDirection::None) {
        qDebug() << __FUNCTION__ << "double set seat direction";
        return;
    }


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


    // 按钮
    touchCard_ = new QPushButton();
    analysisOutCard_ = new QPushButton();
    analysisCard_ = new QPushButton();
    touchCard_->setText(QStringLiteral("摸牌"));
    analysisOutCard_->setText(QStringLiteral("建议出牌"));
    analysisCard_->setText(QStringLiteral("分析牌型"));
    touchCard_->setStyleSheet("QWidget{background-color:white;}" \
                              "QWidget:hover{border: 3px solid red;}");
    analysisOutCard_->setStyleSheet("QWidget{background-color:white;}" \
                              "QWidget:hover{border: 3px solid red;}");
    analysisCard_->setStyleSheet("QWidget{background-color:white;}" \
                              "QWidget:hover{border: 3px solid red;}");

    connect(touchCard_,SIGNAL(clicked()),
            this,SLOT(onTouchCard()));
    connect(analysisOutCard_,SIGNAL(clicked()),
            this,SLOT(onAnalysisOutCard()));
    connect(analysisCard_,SIGNAL(clicked()),
            this,SLOT(onAnalysisCard()));

    QVBoxLayout* hlayout = new QVBoxLayout();
    hlayout->addWidget(touchCard_);
    hlayout->addWidget(analysisOutCard_);
    hlayout->addWidget(analysisCard_);

    layout->addLayout(hlayout);

    setLayout(layout);

}

void SRMahjongSeatWidget::onPlayerOutCard(enDirection drc, unsigned char data)
{
    // 若自己打出则不分析
    if (drc == direction_)
        return;

    int ret_action = robot_->getAction(drc,data);

    if (ret_action == WIK_GANG || ret_action == WIK_PENG) {
        setStyleSheet("QWidget{background-color:rgb(0,255,122);}" \
                      "QWidget:hover{border: 3px solid red;}");
        for (auto item : listMahjong_) {
            if (data == item->getCardData()) {
                QPen pen(QColor(255,2,0));
                item->setPen(&pen);
            }
        }
    }
    else if (ret_action == WIK_CHI_HU) {
        setStyleSheet("QWidget{background-color:rgb(255,0,0);}" \
                      "QWidget:hover{border: 3px solid red;}");
    }
    else {
//        setStyleSheet("QWidget{background-color:green;}" \
//                      "QWidget:hover{border: 1px solid red;}");

    }

    return;

}

void SRMahjongSeatWidget::touchCard(unsigned char data)
{
    robot_->touchCard(data);
    upMahjongBox();

//    unsigned char out_card[MAX_COUNT] = {};
//    unsigned char card_size = 0;
//    int ret_action = robot_->getOutCard(out_card,&card_size);

//    if (ret_action == WIK_NULL) {
//        emit sigOutCard(direction_,out_card[0]);
//    }
//    else if (ret_action == WIK_CHI_HU) {
//        emit sigHu(direction_);
//        setStyleSheet("QWidget{background-color:red;}" \
//                      "QWidget:hover{border: 1px solid green;}");
//        return;
//    }
//    else {
//        emit sigAction(direction_,ret_action,out_card[0]);
//    }

//    // 删除计划打出的牌
//    for (auto item : listMahjong_) {
//        if (card_size == 0)
//            break;
//        if (item->getCardData() == out_card[0]) {
//            item->setStyleSheet("QWidget{background-color:red;}" \
//                       "QWidget:hover{border: 1px solid black;}");
//            --card_size;
//        }
//    }

//    upMahjongBox();

}

void SRMahjongSeatWidget::onMahjongKnockout(QWidget *object)
{
    SRMahjongWidget * temp_object = qobject_cast<SRMahjongWidget*>(object);

    if (temp_object == nullptr || mahjong_ == nullptr) {
        qDebug() << __FUNCTION__ << "convert widget error.";
        return ;
    }

    // 打出这张牌
    lastOutCard_ = temp_object->getCardData();
    mahjong_->delCard(temp_object->getCardData());
    emit sigOutCard(direction_,temp_object->getCardData());
    temp_object->onModifyData(0);


    upMahjongBox();

}
void SRMahjongSeatWidget::onTouchCard()
{
    robot_->touchCard(hallWidget_->getOneCard());
    upMahjongBox();
    qDebug() << direction_ << "onTouchCard";

}

void SRMahjongSeatWidget::onAnalysisOutCard()
{
    for (auto item : listMahjong_)
        item->setStyleSheet("QWidget{background-color:none;}" \
                            "QWidget:hover{border: 1px solid green;}");


    unsigned char arrcard[MAX_COUNT] = {};
    unsigned char uc_count = 0;
    int ret = robot_->analysisOutCard(arrcard,&uc_count);
    if (ret == WIK_CHI_HU) {
        setStyleSheet("QWidget{background-color:red;}" \
                      "QWidget:hover{border: 1px solid green;}");
    }
    else if (ret == WIK_LISTEN) {
        setStyleSheet("QWidget{background-color:yellow;}" \
                      "QWidget:hover{border: 1px solid red;}");
    }
    else {
        setStyleSheet("QWidget{background-color:green;}" \
                      "QWidget:hover{border: 1px solid red;}");
    }

    for (int i = 0; i < uc_count; ++i) {
        for(auto item : listMahjong_) {
            if (arrcard[i] == item->getCardData())
                item->setPen(&QPen(QColor(255,0,0),5));

        }
    }

}

void SRMahjongSeatWidget::onAnalysisCard()
{
    for (auto item : listMahjong_)
        item->setStyleSheet("QWidget{background-color:red;}" \
                            "QWidget:hover{border: 1px solid green;}");


    unsigned char arrcard[MAX_COUNT] = {};
    unsigned char uc_count = 0;
    robot_->analysisCard(arrcard,&uc_count);

    for (int i = 0; i < uc_count; ++i) {
        for(auto item : listMahjong_) {
            if (arrcard[i] == item->getCardData())
                item->setPen(&QPen(QColor(0,0,255),5));

        }
    }
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


