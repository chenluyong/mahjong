#include "srmainwindow.h"
#include "srmahjongtablewidget.h"

#include <QAction>
#include <QDebug>
#include <QLabel>
#include <QToolBar>
#include <QMenu>
#include <QStatusBar>
#include <QMenuBar>
#include <QBoxLayout>

SRMainWindow::SRMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setStyleSheet("background-color:rgb(1,15,5);");
    // 设置中心窗口
    mahjongTable_ = new SRMahjongTableWidget();
    setCentralWidget(mahjongTable_);

    // 初始化基础框架
    setMinimumSize(1000,800);
    initMenu();

}

SRMainWindow::~SRMainWindow()
{
}


void SRMainWindow::initMenu()
{
    // 建立标签
    QAction* openAction = new QAction(tr("&Start"), this);
    openAction->setStatusTip(tr("Game Start."));
    openAction->setIcon(QIcon(":/images/UI_UI_BMP_FILE_SELECTOR_DISABLED_DIRECTORY.png"));

    connect(openAction,SIGNAL(triggered()),mahjongTable_,SLOT(onOpen()));

    QLabel* msgLabel = new QLabel;
    msgLabel->setMaximumSize(msgLabel->sizeHint());
    msgLabel->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(msgLabel);
    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

    /*
     *QMainWindow有一个menuBar()函数，会返回菜单栏，。如果不存在会自动创建，如果已经存在就返回那个菜 单栏的指针。
     *直接使用返回值添加一个菜单，也就是addMenu，参数是一个QString，也就是显示的菜单名字。然后使用这个QMenu指针添加这个QAction。
     */
    QMenu *file = menuBar()->addMenu(tr("&Game"));
    file->setStatusTip(tr("Game Action"));
    file->addAction(openAction);
}
