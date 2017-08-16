#ifndef SRPLAYERDESKTOP_H
#define SRPLAYERDESKTOP_H
#include "stdafx.h"
#include <QBoxLayout>
#include <QWidget>
class QPushButton;
class SRPlayerModel;
class SRCardModel;

class SRPlayerDesktop : public QWidget
{
    Q_OBJECT
public:
    explicit SRPlayerDesktop(QWidget *parent = nullptr);

    // 获得数据模型
    SRPlayerModel* getModel(void) { return model_; }

    // 方向的设置
    void setDirection(enDirection _direction);
    enDirection getDirection(void);

    // 获得方向的字符串
    const char * const getDirectionName();


protected:

    virtual void paintEvent(QPaintEvent *event);


signals:

    // 出牌信号
    void emOutCard(enDirection direction, BYTE data);

    // 胡
    void emHu(enDirection direction, BYTE* data, BYTE count);
    // 碰
    void emPeng(enDirection direction, BYTE* data, BYTE count);
    // 吃
    void emChi(enDirection direction, BYTE* data, BYTE count);

public slots:

    // 发牌
    void onDealCard(enDirection direction, BYTE *data, BYTE count);

    // 出牌
    void onPlayState(BYTE data);

    // 其他人的出牌
    void onOtherPlayState(enDirection direction, BYTE data);

private:

    SRPlayerModel* model_;

    SRCardModel* cardModel_;

    QList<QPushButton*> listButton_;

    QBoxLayout* layout_;

};

#endif // SRPLAYERDESKTOP_H
