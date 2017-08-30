#ifndef SRMAHJONGWIDGET_H
#define SRMAHJONGWIDGET_H

#include <QWidget>
class QPen;
class QLabel;
class SRMahjongWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SRMahjongWidget(QWidget *parent = nullptr);

public:

    unsigned char getCardData(void) {return cardData_;}

public:

    bool operator< (const SRMahjongWidget& other);
    bool operator> (const SRMahjongWidget& other);
    bool operator==(const SRMahjongWidget& other);


public:

    static QString convertToText(unsigned char data);

    void setPen(QPen* pen);

signals:


    void sigDoubleClick(QWidget*);

public slots:

    void onModifyData(unsigned char data);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    /**
     * @brief : 鼠标进入窗口事件
     */
    virtual void enterEvent(QEvent *event);
    /**
     * @brief : 鼠标离开窗口事件
     */
    virtual void leaveEvent(QEvent *event);



    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:



private:


    QString info_;

    unsigned char cardData_;

    /*  当前展示的图标  */
    QPixmap currentPix_;

    /*  图标列表  */
    QList<QPixmap> listPix_;

    QPen* pen_;

};

#endif // SRMAHJONGWIDGET_H
