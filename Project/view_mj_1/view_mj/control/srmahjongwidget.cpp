#include "srmahjongwidget.h"
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QPixmap>

#include <SRMahjong.h>

SRMahjongWidget::SRMahjongWidget(QWidget *parent)
    : QWidget(parent) {
    setStyleSheet("QWidget{background-color:orange;}" \
                  "QWidget:hover{border: 3px solid black;}");

    setMaximumSize(55,74);
    cardData_ = 0;
    pen_ = new QPen(QColor(0,174,255),3);

    //  链式动画图标分离
    QPixmap temp_pix(":/ico/mahjong");
    const int& IMAGE_COUNT = 35;
    for(int i = 0; i != IMAGE_COUNT; ++i) {
        listPix_.append(temp_pix.copy(i * (temp_pix.width() / IMAGE_COUNT), 0,
                        temp_pix.width() / IMAGE_COUNT, temp_pix.height()));
    }

    currentPix_ = listPix_.at(34);
    this->setGeometry(0,0,currentPix_.width(),currentPix_.height());
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


unsigned char switchToCardIndex(unsigned char cbCardData) {
    return ((cbCardData&MASK_COLOR) >> 4) * 9 + (cbCardData&MASK_VALUE) - 1;
}


void SRMahjongWidget::onModifyData(unsigned char data)
{
    cardData_ = data;

    if (data == 0)
        currentPix_ = listPix_.at(34);
    if (switchToCardIndex(data) < 35)
        currentPix_ = listPix_.at(switchToCardIndex(data));

//    info_ = convertToText(data);
}

void SRMahjongWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,
                           &opt, &p, this);
    p.drawPixmap(rect(), currentPix_);

    /// 绘制边框线
    p.setPen(*pen_);
    p.drawRect(rect());

    p.drawText(0,0,width(),height(),Qt::AlignCenter,info_);


}

void SRMahjongWidget::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

}

void SRMahjongWidget::enterEvent(QEvent *event)
{
    pen_->setColor(QColor(255,0,0));
}

void SRMahjongWidget::leaveEvent(QEvent *event)
{
    pen_->setColor(QColor(0,0,0,0));
}

void SRMahjongWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setStyleSheet("QWidget{background-color:orange;}" \
                      "QWidget:hover{border: 3px solid black;}");
        emit sigDoubleClick(this);
//        qDebug() <<  info_ << QStringLiteral("被双击");
    }
}

QString SRMahjongWidget::convertToText(unsigned char data)
{

    if (!isValidCard(data))
        return "??? ";


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
    case 0x30: {
        name = QStringLiteral("风");
        if (card_num == 1)
            name = QStringLiteral("东") + name;
        else if (card_num == 2)
            name = QStringLiteral("南") + name;
        else if (card_num == 3)
            name = QStringLiteral("西") + name;
        else if (card_num == 4)
            name = QStringLiteral("北") + name;
        else if (card_num == 5)
            name = QStringLiteral("中");
        else if (card_num == 6)
            name = QStringLiteral("发");
        else if (card_num == 7)
            name = QStringLiteral("白");
        return name;
    }
    default:
        break;
    }

    name = QString::number(card_num) + name;

    return name;
}

void SRMahjongWidget::setPen(QPen *pen)
{
    *pen_ = *pen;
}
