#ifndef SRMAINWIDGET_H
#define SRMAINWIDGET_H
#include "stdafx.h"
#include <QWidget>
class SRDesktop;
class SRPlayerDesktop;
class SRMainWidget : public QWidget
{
    Q_OBJECT

public:
    SRMainWidget(QWidget *parent = 0);
    ~SRMainWidget();

private slots:

private:

    // 基础服务布局
    void initServe(void);

private:
    SRPlayerDesktop* player_[4];
    SRDesktop* desktop_;
};

#endif // SRMAINWIDGET_H
