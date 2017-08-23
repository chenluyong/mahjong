#include "srmainwindow.h"
#include <QApplication>
#include <QDebug>
#include "control/srmahjongwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SRMainWindow w;
    w.show();

    return a.exec();
}
