#ifndef SRMAHJONGSEATWIDGET_H
#define SRMAHJONGSEATWIDGET_H

#include <QList>
#include <QWidget>

class SRMahjongWidget;

class SRMahjongSeatWidget : public QWidget
{
    Q_OBJECT
public:

    enum enDirection {
        South = 0,              //南向 下
        West,					//西向 左
        North,                  //北向 上
        East,					//东向 右
    };

    explicit SRMahjongSeatWidget(QWidget *parent = nullptr);

signals:

public:

    void setDirection(enDirection drc);

public slots:


protected:

    virtual void paintEvent(QPaintEvent *event);

private:



private:
    // 一个位子应该具备方向
    enDirection direction_;

    QList<SRMahjongWidget*> listMahjong_;

};

#endif // SRMAHJONGSEATWIDGET_H
