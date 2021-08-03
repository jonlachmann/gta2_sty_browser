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
    void OnMenuFileNewClick() {qDebug() << "MainMenu/File/New";}
    void OnMenuFileOpenClick();
    void OnMenuFileSaveClick() {qDebug() << "MainMenu/File/Save";}
    void OnMenuFileSaveAsClick() {qDebug() << "MainMenu/File/SaveAs";}
    void OnMenuFileCloseClick() {qDebug() << "MainMenu/File/Close";}
    void OnMenuEditUndoClick() {qDebug() << "MainMenu/Edit/Undo";}
    void OnMenuEditRedoClick() {qDebug() << "MainMenu/Edit/Redo";}
    void OnMenuEditCutClick() {qDebug() << "MainMenu/Edit/Cut";}
    void OnMenuEditCopyClick() {qDebug() << "MainMenu/Edit/Copy";}
    void OnMenuEditPasteClick() {qDebug() << "MainMenu/Edit/Paste";}
    void OnMenuEditDeletaClick() {qDebug() << "MainMenu/Edit/delete";}
    void OnMenuEditSelectAllClick() {qDebug() << "MainMenu/Edit/SelectAll";}
    void OnMenuHelpAboutClick() {QMessageBox::about(this, "About", "MainMenu example.\nVersion 1.0.0\n\n@ 2020 by Gammasoft.");}

    QFrame frame;
    //QLabel label1 {&panel};
};


#endif //STY_BROWSER_MAINWINDOW_H
