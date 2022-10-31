#include <QApplication>
#include <QDebug>
#include <QMainWindow>

#include "clock.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow w;
    w.resize(600, 600);

    Clock* clock = new Clock(&w);
    w.setCentralWidget(clock);
    w.show();

    return a.exec();
}
