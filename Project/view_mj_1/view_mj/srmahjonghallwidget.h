#ifndef SRMAHJONGHALLWIDGET_H
#define SRMAHJONGHALLWIDGET_H

#include <srmjglobal.h>

#include <QWidget>

class SRInvisibleMahjongPool;
class SRVisibleMahjongPool;

class SRMahjongHallWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SRMahjongHallWidget(QWidget *parent = nullptr);

public:

    // 获得牌墙上的牌
    SRInvisibleMahjongPool* getInvisibleMahjongPool(void) {return invisibleMahjongPool_;}
    // 获得牌池中的牌
    SRVisibleMahjongPool* getVisibleMahjongPool(void) {return visibleMahjongPool_;}

signals:
    // 发牌
    void sigDealCard(enDirection direction, BYTE data);

    // 玩家出牌
    void sigOutCard(enDirection direction, BYTE data);

public slots:

    // 洗牌
    void onShuffle(void);
    // 发牌
    // direction ：发给谁
    // count : 发多少张
    void onDealCard(enDirection direction, BYTE count);

    // 用户出牌，牌插入牌池之中
    void onOutCardToVisibleMahjongPool(enDirection direction, unsigned char card);


protected:

    virtual void paintEvent(QPaintEvent *event);

private:
    SRInvisibleMahjongPool* invisibleMahjongPool_;
    SRVisibleMahjongPool* visibleMahjongPool_;

    unsigned char cbCardDataArray_[MAX_REPERTORY + 1];
};

#endif // SRMAHJONGHALLWIDGET_H
