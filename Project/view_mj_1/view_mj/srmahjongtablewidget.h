#ifndef SRMAHJONGTABLEWIDGET_H
#define SRMAHJONGTABLEWIDGET_H

#include <srmjglobal.h>
#include <QWidget>

class SRMahjongSeatWidget;
class SRMahjongHallWidget;
class SRRobot;
class SRMahjongTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SRMahjongTableWidget(QWidget *parent = nullptr);

signals:

public slots:

    void onOpen(void);

    // Íæ¼ÒÐÐÎª £¨³Ô ºú Åö£©
    void onPlayerAction(enDirection direction, int action);

    void onDealCard(enDirection direction, BYTE data);

protected:

    virtual void paintEvent(QPaintEvent *event);


private:


    void initSeatLayout(void);

private:
    SRMahjongHallWidget* hallWidget_;
    SRMahjongSeatWidget* seatWidget_[4];

    SRRobot* robot_[4];


};

#endif // SRMAHJONGTABLEWIDGET_H
