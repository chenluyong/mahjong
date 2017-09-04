#ifndef SRMAHJONGSEATWIDGET_H
#define SRMAHJONGSEATWIDGET_H
#include "srmjglobal.h"
#include <QList>
#include <QWidget>
class QPushButton;
class SRMahjongWidget;
class SRMahjongHallWidget;
class SRMahjongSeatHallWidget;

class SRRobot;
class SRMahjong;

class SRMahjongSeatWidget : public QWidget
{
    Q_OBJECT
public:


    explicit SRMahjongSeatWidget(QWidget *parent = nullptr);

    void setRobot(SRRobot* robot);

    void setMahjong(SRMahjong* mahjong);

    void setHallWidget(SRMahjongHallWidget* hall) {hallWidget_ = hall;}

signals:
    // 出牌信号
    void sigOutCard(enDirection direction, unsigned char data);

    // 胡
    void sigHu(enDirection direction);

    // 其他行为 左吃 中吃 右吃 碰 杠
    void sigAction(enDirection direction, int action, unsigned char data);
public:

    void setDirection(enDirection drc);
    enDirection getDirection(void) {return direction_;}


    // 摸牌
    void touchCard(unsigned char data);

    void upMahjongBox(void);
public slots:


    // 玩家出牌
    void onPlayerOutCard(enDirection drc, unsigned char data);

private slots:

    // 被双击 - 出牌的意思
    void onMahjongKnockout(QWidget* object);

    void onTouchCard(void);
    void onAnalysisOutCard(void);
    void onAnalysisCard(void);

protected:
    // 出牌
    void mahjongKnockout(unsigned char card);

    virtual void paintEvent(QPaintEvent *event);



private:
    SRMahjongHallWidget* hallWidget_;

    // 一个位子应该具备方向
    enDirection direction_;

    QList<SRMahjongWidget*> listMahjong_;

    unsigned char lastOutCard_;

    SRRobot* robot_;

    SRMahjong * mahjong_;

    QPushButton* touchCard_;
    QPushButton* analysisOutCard_;
    QPushButton* analysisCard_;

    SRMahjongSeatHallWidget* seatHallWidget_;
};

#endif // SRMAHJONGSEATWIDGET_H
