#include "srmainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SRMainWidget w;
    w.show();

    return a.exec();
}
