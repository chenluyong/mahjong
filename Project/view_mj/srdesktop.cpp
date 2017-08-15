#include "srdesktop.h"
#include "srcommon.h"
#include <time.h>

#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
SRDesktop::SRDesktop(QWidget *parent) : QWidget(parent) {
    memset(cbCardDataArray_,0,sizeof(cbCardDataArray_));
    darkCardLayout_ = new QGridLayout;
    brightnessCardLayout_ = new QGridLayout;
    layout_ = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
    layout_->addLayout(darkCardLayout_);
    layout_->addLayout(brightnessCardLayout_);

    setLayout(layout_);
}

void SRDesktop::onShuffle() {
    // 数据清理
    memset(cbCardDataArray_,0,sizeof(cbCardDataArray_));
    index_ = 0;

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



        // 临时加的 桌面明牌
        QSharedPointer<QPushButton> temp_btn(new QPushButton(
                           sr::convertCard(cbCardDataArray_[cbRandCount - 1])));

        listDarkPushButton_.push_back(temp_btn);
        temp_btn->setFixedSize(50,50);
        temp_btn->setStyleSheet("QPushButton{background:red;}");
    } while (cbRandCount < cbMaxCount);


    int r = 0, c = 0;

    for (auto& btn : listDarkPushButton_) {

        darkCardLayout_->addWidget(btn.data(),r,c++,1,1);
        if (c > 10) {
            c = 0;
            r++;
        }

    }

    return;
}

void SRDesktop::onDealCard(enDirection direction, BYTE count) {
    if ((index_ + count) >= MAX_REPERTORY)
        emit emDealCard(direction, &cbCardDataArray_[index_], MAX_REPERTORY - index_);
    else
        emit emDealCard(direction, &cbCardDataArray_[index_], count);

    index_ += count;

    for(int i = 0; i < count; ++i) {
        darkCardLayout_->removeWidget(listDarkPushButton_.front().data());
        listDarkPushButton_.pop_front();
    }

    if (index_ >= MAX_REPERTORY)
        qDebug() << "card use up.";
}

void SRDesktop::onStart()
{
    onDealCard(enDirection::West, 1);
}
