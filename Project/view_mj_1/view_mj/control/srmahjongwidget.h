#ifndef SRMAHJONGWIDGET_H
#define SRMAHJONGWIDGET_H

#include <QWidget>
class QLabel;
class SRMahjongWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SRMahjongWidget(QWidget *parent = nullptr);

public:

    bool operator< (const SRMahjongWidget& other);
    bool operator> (const SRMahjongWidget& other);
    bool operator==(const SRMahjongWidget& other);

signals:


    void sigDoubleClick(QWidget*);

public slots:
    void onModifyData(unsigned char data);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:


    QString convertToText(unsigned char data);

private:


    QString info_;

    unsigned char cardData_;

};

#endif // SRMAHJONGWIDGET_H
