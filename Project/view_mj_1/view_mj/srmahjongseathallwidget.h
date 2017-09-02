#ifndef SRMAHJONGSEATHALLWIDGET_H
#define SRMAHJONGSEATHALLWIDGET_H

#include <srmjglobal.h>

#include <QWidget>

class SRMahjongWidget;
class QGridLayout;
class SRMahjongSeatHallWidget;


class SRMahjongSeatHallWidget : public QWidget
{

    Q_OBJECT

public:

    explicit SRMahjongSeatHallWidget(QWidget *parent = nullptr);

    void push_back(SRMahjongWidget* mahjong);

    void setDirection(enDirection drc);
protected:

    virtual void paintEvent(QPaintEvent *event);
signals:



public slots:


private:
    enDirection direction_;

    QList<SRMahjongWidget*> listMahjong_;
    QGridLayout* layout_;

    int row_;
    int col_;

};

#endif // SRMAHJONGSEATHALLWIDGET_H
