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

    SRPlayerModel* getModel(void) { return model_; }

    void setDirection(enDirection _direction);
    enDirection getDirection(void);

    const char * const getDirectionName();
signals:

public slots:

    void onDealCard(enDirection direction, BYTE *data, BYTE count);

private:
    SRPlayerModel* model_;

    SRCardModel* cardModel_;

    QList<QPushButton*> listButton_;

    QBoxLayout* layout_;

};

#endif // SRPLAYERDESKTOP_H
