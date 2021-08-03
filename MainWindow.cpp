//
// Created by Jon Lachmann on 2021-08-03.
//

#include <QFileDialog>
#include "MainWindow.h"
#include "QSprite.h"

MainWindow::MainWindow() {
    QMenu* menuFile = menuBar()->addMenu("&File");
    menuFile->addAction(style()->standardIcon(QStyle::StandardPixmap::SP_DirOpenIcon), "&Open .sty", this, &MainWindow::OnMenuFileOpenClick, QKeySequence(Qt::CTRL + Qt::Key_O));
    menuFile->addSeparator();
    menuFile->addAction("&Exit", this, &MainWindow::OnMenuFileCloseClick, QKeySequence(Qt::ALT + Qt::Key_F4));

    QMenu* menuEdit = menuBar()->addMenu("&Category");
    menuEdit->addAction("&Cars", this, &MainWindow::OnMenuCategoryCarsClick, QKeySequence(Qt::CTRL + Qt::Key_Z));
    menuEdit->addAction("&Peds", this, &MainWindow::OnMenuCategoryPedsClick, QKeySequence(Qt::CTRL + Qt::Key_Y));

    QMenu* menuHelp = menuBar()->addMenu("&Help");
    menuHelp->addAction("&About", this, &MainWindow::OnMenuHelpAboutClick);


    setWindowTitle("GTA2 STY Browser");
    layout = new QVBoxLayout;
    hlayout = new QHBoxLayout();

    QWidget *widg = new QWidget();
    widg->setLayout(hlayout);

    scroll = new QScrollArea();

    scroll->setWidget(widg);
    scroll->setWidgetResizable(true);

    QWidget *window = new QWidget();
    window->setLayout(layout);

    setCentralWidget(scroll);

    resize(800, 600);
}

void MainWindow::OnMenuFileOpenClick() {
    QString file1Name = QFileDialog::getOpenFileName(this,
                                             tr("Open .sty File "), "~", tr("STY Files (*.sty)"));
    std::cout << file1Name.toStdString() << std::endl;

    styleFile.load(file1Name.toStdString(), true);

    for (int i = 0; i < 10; i++) {
        QSprite sprite = QSprite(&styleFile);
        sprite.setSprite(i);
        QLabel *myLabel = new QLabel(this);
        myLabel->setPixmap(QPixmap::fromImage(sprite.getImage()));
        hlayout->addWidget(myLabel);
    }
}

void MainWindow::test() {
    styleFile.load("../../openGTA2/data/ste.sty", true);

    for (int i = 0; i < 100; i++) {
        QSprite sprite = QSprite(&styleFile);
        sprite.setSprite(i);
        QLabel *myLabel = new QLabel(this);
        myLabel->setPixmap(QPixmap::fromImage(sprite.getImage()));
        hlayout->addWidget(myLabel);
    }
}

void MainWindow::OnMenuCategoryCarsClick() {
    QList<QWidget*> children = scroll->findChildren<QWidget*>();
    qDebug() << children.size();
    //foreach (QWidget* child, children) delete child;

    for (int i = 20; i < 100; i++) {
        QSprite sprite = QSprite(&styleFile);
        sprite.setSprite(i);
        QLabel *myLabel = new QLabel(this);
        myLabel->setPixmap(QPixmap::fromImage(sprite.getImage()));
        hlayout->addWidget(myLabel);
    }
}

void MainWindow::OnMenuCategoryPedsClick() {

}
