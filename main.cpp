#include <QApplication>
#include <QIcon>
#include <QStringList>
#include <QVBoxLayout>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    MainWindow window1;
    window1.show();

    //window1.test();

    return application.exec();
}