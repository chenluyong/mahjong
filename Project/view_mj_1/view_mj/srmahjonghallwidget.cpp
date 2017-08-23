#include "srmahjonghallwidget.h"

#include <QStyleOption>
#include <QPainter>
SRMahjongHallWidget::SRMahjongHallWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("QWidget{background-color:white;}" \
                  "QWidget:hover{border: 1px solid black;}");
}

void SRMahjongHallWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,
                           &opt, &p, this);

}
