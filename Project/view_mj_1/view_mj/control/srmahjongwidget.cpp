#include "srmahjongwidget.h"
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>

SRMahjongWidget::SRMahjongWidget(QWidget *parent)
    : QWidget(parent) {
    setStyleSheet("QWidget{background-color:orange;}" \
                  "QWidget:hover{border: 3px solid black;}");

    info_ = QStringLiteral("暂无");
#ifndef QT_NO_DEBUG
    unsigned char i = rand() % 30;
    onModifyData(i);
#endif

    setMaximumSize(55,74);
}

bool SRMahjongWidget::operator<(const SRMahjongWidget &other)
{
    return (this->cardData_ < other.cardData_);
}

bool SRMahjongWidget::operator>(const SRMahjongWidget &other)
{
    return (this->cardData_ > other.cardData_);
}

bool SRMahjongWidget::operator==(const SRMahjongWidget &other)
{
    return (this->cardData_ == other.cardData_);
}


void SRMahjongWidget::onModifyData(unsigned char data)
{
    cardData_ = data;
    info_ = convertToText(data);
}

void SRMahjongWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,
                           &opt, &p, this);

    p.drawText(0,0,width(),height(),Qt::AlignCenter,info_);


}

void SRMahjongWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit sigDoubleClick(this);
        qDebug() <<  info_ << QStringLiteral("被双击");
    }
}

QString SRMahjongWidget::convertToText(unsigned char data)
{

    unsigned char card_type = data & 0xF0;

    unsigned char card_num = data & 0x0F;

    QString name;

    switch (card_type) {
    case 0x00:
        name = QStringLiteral("万");
        break;
    case 0x10:
        name = QStringLiteral("条");
        break;
    case 0x20:
        name = QStringLiteral("同");
        break;
    case 0x30:
        name = QStringLiteral("风");
        break;
    default:
        break;
    }

    name = QString::number(card_num) + name;

    return name;
}
