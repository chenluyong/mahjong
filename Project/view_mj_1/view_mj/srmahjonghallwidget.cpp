#include "srmahjonghallwidget.h"
#include "control/srmahjongwidget.h"
#include "srmahjongseathallwidget.h"

#include <time.h>

#include <QStyleOption>
#include <QPainter>
#include <QList>
#include <QGridLayout>
#include <QDebug>

#include <srmjglobal.h>
#include <SRMahjongPool.h>



SRMahjongHallWidget::SRMahjongHallWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("QWidget{border-image:url(:/bg/background);}" \
                  "QWidget:hover{border: 1px solid black;}");

    invisibleMahjongPool_ = new SRInvisibleMahjongPool();
    visibleMahjongPool_ = new SRVisibleMahjongPool();


    for (int i = (int)enDirection::South; i <= (int)enDirection::East; ++i) {
        seatHallWidget_[i] = new SRMahjongSeatHallWidget;
        seatHallWidget_[i]->setDirection((enDirection)i);
    }


}

unsigned char SRMahjongHallWidget::getOneCard()
{
    unsigned char ret_card = invisibleMahjongPool_->pop_front();

    if (listInvisibleMahjong_.front()->getCardData() != ret_card)
        qDebug() << __FUNCTION__ <<": warning!";

    listInvisibleMahjong_.front()->hide();
    listUndefineMahjong_.push_back(listInvisibleMahjong_.front());
    invisibleLayout_->removeWidget(listInvisibleMahjong_.front());
    listInvisibleMahjong_.pop_front();


    return ret_card;
}

void SRMahjongHallWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,
                           &opt, &p, this);

}




void SRMahjongHallWidget::onShuffle() {

    // 数据清理
    memset(cbCardDataArray_,0,sizeof(cbCardDataArray_));

    // 初始化种子
    srand(time(0));

    // 混乱准备
    BYTE cbMaxCount = MAX_REPERTORY;
    BYTE cbCardDataTemp[sizeof(g_cbCardDataArray)];
    memcpy(cbCardDataTemp, g_cbCardDataArray, sizeof(g_cbCardDataArray));

    // 混乱扑克
    BYTE cbRandCount=0,cbPosition=0;
    do {
        cbPosition = rand() % (cbMaxCount-cbRandCount);
        cbCardDataArray_[cbRandCount++] = cbCardDataTemp[cbPosition];
        cbCardDataTemp[cbPosition] = cbCardDataTemp[cbMaxCount-cbRandCount];
    } while (cbRandCount < cbMaxCount);

    // 设置到牌墙之上
    invisibleMahjongPool_->setMahjongPoolData(cbCardDataArray_, cbRandCount);

    // 洗好牌
    for (int i = 0; i < cbRandCount; ++i) {
        SRMahjongWidget* tmp_mj = new SRMahjongWidget();
        tmp_mj->onModifyData(cbCardDataArray_[i]);
        tmp_mj->setFixedSize(33,33);
        listUndefineMahjong_.push_back(tmp_mj);
    }



    QGridLayout* layout = new QGridLayout;
    // 北向
    layout->addWidget(seatHallWidget_[enDirection::North],
            0,2,2,7);
    // 西向
    layout->addWidget(seatHallWidget_[enDirection::West],
            2,0,7,2);
    // 南向
    layout->addWidget(seatHallWidget_[enDirection::South],
            9,2,2,7);
    // 东向
    layout->addWidget(seatHallWidget_[enDirection::East],
            2,9,7,2);
    invisibleLayout_ = new QGridLayout();
    layout->addLayout(invisibleLayout_,3,3,4,4);


    // 将散乱的牌堆成牌墙
    auto mahjong_iter = listUndefineMahjong_.begin();
    for (int r = 0; r < 12; ++r) {
        for (int c = 0; c < 12; ++c) {
            invisibleLayout_->addWidget(*mahjong_iter,r,c);
            listInvisibleMahjong_.push_back(*mahjong_iter);
            mahjong_iter = listUndefineMahjong_.erase(mahjong_iter);
            if ((mahjong_iter) == listUndefineMahjong_.end()) {
                break;
            }
        }

        if (mahjong_iter == listUndefineMahjong_.end()) {
            break;
        }
    }


    setLayout(layout);


    return;
}

void SRMahjongHallWidget::onDealCard(enDirection direction, BYTE count) {
    for (BYTE i = 0; i < count; ++i) {
        unsigned char temp_card = invisibleMahjongPool_->pop_front();
        emit sigDealCard(direction, temp_card);
        auto pfront = listInvisibleMahjong_.front();
        if (pfront->getCardData() != temp_card) {
            qDebug() << __FUNCTION__ << ": warning!";
        }

        // 删除发出的牌
        pfront->hide();
        QPen pen(QColor(255,255,0));
        pfront->setPen(&pen);
        invisibleLayout_->removeWidget(pfront);
        listUndefineMahjong_.push_back(pfront);
        listInvisibleMahjong_.pop_front();
    }
}

void SRMahjongHallWidget::onOutCardToVisibleMahjongPool(enDirection direction,
                                                        unsigned char card) {

    for (auto item = listUndefineMahjong_.begin();
         item != listUndefineMahjong_.end();++item) {
        if ((*item)->getCardData() == card) {
            (*item)->show();
            seatHallWidget_[direction]->push_back(*item);
            listVisibleMahjong_.push_back(*item);
            listUndefineMahjong_.erase(item);
            break;
        }
    }

    visibleMahjongPool_->push_back(card);
}
