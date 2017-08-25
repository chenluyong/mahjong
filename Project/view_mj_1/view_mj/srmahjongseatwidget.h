#ifndef SRMAHJONGSEATWIDGET_H
#define SRMAHJONGSEATWIDGET_H
#include "srmjglobal.h"
#include <QList>
#include <QWidget>

class SRMahjongWidget;
class SRRobot;
class SRMahjong;
class SRMahjongSeatWidget : public QWidget
{
    Q_OBJECT
public:


    explicit SRMahjongSeatWidget(QWidget *parent = nullptr);

    void setRobot(SRRobot* robot);

    void setMahjong(SRMahjong* mahjong);

signals:
    // 出牌信号
    void sigOutCard(enDirection direction, unsigned char data);

    // 胡
    void sigHu(enDirection direction);

    // 其他行为 左吃 中吃 右吃 碰 杠
    void sigAction(enDirection direction, int action, BYTE data);
public:

    void setDirection(enDirection drc);
    enDirection getDirection(void) {return direction_;}


    // 摸牌
    void touchCard(unsigned char data);

public slots:


    // 玩家出牌
    void onPlayerOutCard(unsigned char data);

private slots:

    // 被双击 - 出牌的意思
    void onMahjongKnockout(QWidget* object);

protected:

    virtual void paintEvent(QPaintEvent *event);

private:

    void upMahjongBox(void);

private:
    // 一个位子应该具备方向
    enDirection direction_;

    QList<SRMahjongWidget*> listMahjong_;

    SRRobot* robot_;

    SRMahjong * mahjong_;

};

#endif // SRMAHJONGSEATWIDGET_H
