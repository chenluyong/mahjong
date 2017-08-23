#ifndef SRMAHJONGTABLEWIDGET_H
#define SRMAHJONGTABLEWIDGET_H

#include <QWidget>
class SRMahjongSeatWidget;
class SRMahjongHallWidget;
class SRMahjongTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SRMahjongTableWidget(QWidget *parent = nullptr);

signals:

public slots:
protected:

    virtual void paintEvent(QPaintEvent *event);


private:

    void init(void);

private:
    SRMahjongHallWidget* hallWidget_;
    SRMahjongSeatWidget* seatWidget_[4];

};

#endif // SRMAHJONGTABLEWIDGET_H
