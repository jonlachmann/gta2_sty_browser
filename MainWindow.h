//
// Created by Jon Lachmann on 2021-08-03.
//

#ifndef STY_BROWSER_MAINWINDOW_H
#define STY_BROWSER_MAINWINDOW_H

#include <iostream>
#include <QAction>
#include <QDebug>
#include <QFrame>
#include <QIcon>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QMenuBar>
#include <QLabel>
#include <QStyle>
#include <QVBoxLayout>
#include <QScrollArea>

#include "Style.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    void test();
    QScrollArea *scroll;
    QVBoxLayout *layout;
    QHBoxLayout *hlayout;

    MainWindow();

private:
    void OnMenuFileOpenClick();

    void OnMenuFileCloseClick() {close();}
    void OnMenuCategoryCarsClick();
    void OnMenuCategoryPedsClick();

    void OnMenuHelpAboutClick() {QMessageBox::about(this, "About", "Grand Theft Auto 2 .sty file viewer. \nhttps://github.com/jonlachmann/gta2_sty_browser \nCreated by Jon Lachmann.");}

    QFrame frame;
    Style styleFile;
    //QLabel label1 {&panel};
};


#endif //STY_BROWSER_MAINWINDOW_H
