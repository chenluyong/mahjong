#ifndef SRMAHJONGHALLWIDGET_H
#define SRMAHJONGHALLWIDGET_H

#include <QWidget>

class SRMahjongHallWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SRMahjongHallWidget(QWidget *parent = nullptr);

signals:

public slots:
protected:

    virtual void paintEvent(QPaintEvent *event);
};

#endif // SRMAHJONGHALLWIDGET_H
