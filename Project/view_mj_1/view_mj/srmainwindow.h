#ifndef SRMAINWINDOW_H
#define SRMAINWINDOW_H

#include <memory>
#include <QMainWindow>

class SRMahjongTableWidget;

class SRMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    SRMainWindow(QWidget *parent = 0);
    ~SRMainWindow();

public slots:
    void open(void);

private:
    void initMenu(void);

private:
    SRMahjongTableWidget* mahjongTable_;
};

#endif // SRMAINWINDOW_H
