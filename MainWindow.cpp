//
// Created by Jon Lachmann on 2021-08-03.
//

#include <QFileDialog>
#include "MainWindow.h"
#include "QSprite.h"

MainWindow::MainWindow() {
    QMenu* menuFile = menuBar()->addMenu("&File");
    menuFile->addAction(style()->standardIcon(QStyle::StandardPixmap::SP_FileIcon), "&New", this, &MainWindow::OnMenuFileNewClick, QKeySequence(Qt::CTRL + Qt::Key_N));
    menuFile->addAction(style()->standardIcon(QStyle::StandardPixmap::SP_DirOpenIcon), "&Open", this, &MainWindow::OnMenuFileOpenClick, QKeySequence(Qt::CTRL + Qt::Key_O));
    menuFile->addSeparator();
    menuFile->addAction(style()->standardIcon(QStyle::StandardPixmap::SP_DialogSaveButton), "&Save", this, &MainWindow::OnMenuFileSaveClick, QKeySequence(Qt::CTRL + Qt::Key_S));
    menuFile->addAction("Save &As...", this, &MainWindow::OnMenuFileSaveAsClick);
    menuFile->addSeparator();
    menuFile->addAction(QIcon::fromTheme("document-print"), "&Print", this, &MainWindow::OnMenuFileSaveClick, QKeySequence(Qt::CTRL + Qt::Key_P));
    menuFile->addAction(QIcon::fromTheme("document-print-preview"), "Print preview");
    menuFile->addSeparator();
    menuFile->addAction("&Exit", this, &MainWindow::OnMenuFileCloseClick, QKeySequence(Qt::ALT + Qt::Key_F4));

    QMenu* menuEdit = menuBar()->addMenu("&Category");
    menuEdit->addAction("&Cars", this, &MainWindow::OnMenuEditUndoClick, QKeySequence(Qt::CTRL + Qt::Key_Z));
    menuEdit->addAction("&Peds", this, &MainWindow::OnMenuEditRedoClick, QKeySequence(Qt::CTRL + Qt::Key_Y));
    menuEdit->addSeparator();
    menuEdit->addAction(QIcon::fromTheme("edit-cut"), "&Cut", this, &MainWindow::OnMenuEditCutClick, QKeySequence(Qt::CTRL + Qt::Key_X));
    menuEdit->addAction(QIcon::fromTheme("edit-copy"), "&Copy", this, &MainWindow::OnMenuEditCopyClick, QKeySequence(Qt::CTRL + Qt::Key_C));
    menuEdit->addAction(QIcon::fromTheme("edit-paste"), "&Paste", this, &MainWindow::OnMenuEditPasteClick, QKeySequence(Qt::CTRL + Qt::Key_V));
    menuEdit->addSeparator();
    menuEdit->addAction("Select &All", this, &MainWindow::OnMenuEditSelectAllClick, QKeySequence(Qt::CTRL + Qt::Key_A));

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

    Style style;
    style.load(file1Name.toStdString(), true);

    for (int i = 0; i < 10; i++) {
        QSprite sprite = QSprite(&style);
        sprite.setSprite(i);
        QLabel *myLabel = new QLabel(this);
        myLabel->setPixmap(QPixmap::fromImage(sprite.getImage()));
        hlayout->addWidget(myLabel);
    }
}

void MainWindow::test() {
    Style style;
    style.load("../../openGTA2/data/ste.sty", true);

    for (int i = 0; i < 10; i++) {
        QSprite sprite = QSprite(&style);
        sprite.setSprite(i);
        QLabel *myLabel = new QLabel(this);
        myLabel->setPixmap(QPixmap::fromImage(sprite.getImage()));
        hlayout->addWidget(myLabel);
    }


}