#include <iostream>
#include <ostream>

#include "mainwindow.h"
#include <QApplication>
#include <fstream>
#include <iostream>
#include <unordered_set>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
