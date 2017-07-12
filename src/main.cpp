#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QTime zero(0, 0, 0);
    qsrand(zero.secsTo(QTime::currentTime()));
    return a.exec();
}
