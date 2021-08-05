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
#include "QSprite.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    void test();
    QScrollArea *scroll;
    QVBoxLayout *layout;
    QVBoxLayout *scrollLayout;
    QHBoxLayout *hlayout1;
    QHBoxLayout *hlayout2;
    QHBoxLayout *hlayout3;
    std::vector<std::vector<QLabel*>> sprites;
    int spritesActive = -1; // -1 none, 0 car, 1 ped...

    MainWindow();

private:
    void OnMenuFileOpenClick();

    void OnMenuFileCloseClick() {close();}
    void OnMenuCategoryCarsClick();
    void OnMenuCategoryPedsClick();
    void OnMenuCategoryCodeObjClick();
    void OnMenuCategoryMapObjClick();

    void OnMenuHelpAboutClick() {QMessageBox::about(this, "About", "Grand Theft Auto 2 .sty file viewer. \nhttps://github.com/jonlachmann/gta2_sty_browser \nCreated by Jon Lachmann.");}

    QFrame frame;
    Style styleFile;
    //QLabel label1 {&panel};

    void setSprites(int sprites);

    std::vector<QLabel *> LoadSprites(int base, int nextBase);
};


#endif //STY_BROWSER_MAINWINDOW_H
