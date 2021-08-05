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
    menuEdit->addAction("&Cars", this, &MainWindow::OnMenuCategoryCarsClick, QKeySequence(Qt::CTRL + Qt::Key_1));
    menuEdit->addAction("&Peds", this, &MainWindow::OnMenuCategoryPedsClick, QKeySequence(Qt::CTRL + Qt::Key_2));
    menuEdit->addAction("&Code Objects", this, &MainWindow::OnMenuCategoryCodeObjClick, QKeySequence(Qt::CTRL + Qt::Key_3));
    menuEdit->addAction("&Map Objects", this, &MainWindow::OnMenuCategoryMapObjClick, QKeySequence(Qt::CTRL + Qt::Key_4));
    menuEdit->addAction("&User Objects", this, &MainWindow::OnMenuCategoryUserClick, QKeySequence(Qt::CTRL + Qt::Key_5));
    menuEdit->addAction("&Fonts", this, &MainWindow::OnMenuCategoryFontsClick, QKeySequence(Qt::CTRL + Qt::Key_6));

    QMenu* menuHelp = menuBar()->addMenu("&Help");
    menuHelp->addAction("&About", this, &MainWindow::OnMenuHelpAboutClick);


    setWindowTitle("GTA2 STY Browser");
    layout = new QVBoxLayout;
    scrollLayout = new QVBoxLayout;
    hlayout1 = new QHBoxLayout();
    hlayout2 = new QHBoxLayout();
    hlayout3 = new QHBoxLayout();

    QWidget *scrollLayoutW = new QWidget();
    scrollLayoutW->setLayout(scrollLayout);

    scrollLayout->addLayout(hlayout1);
    scrollLayout->addLayout(hlayout2);
    scrollLayout->addLayout(hlayout3);

    scroll = new QScrollArea();

    scroll->setWidget(scrollLayoutW);
    //scroll->setWidget(hlayout2W);
    //scroll->setWidget(hlayout3W);
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
        hlayout1->addWidget(myLabel);
    }
}

void MainWindow::test() {
    styleFile.load("../../openGTA2/data/ste.sty", true);

    std::cout << styleFile.spriteBase.car << std::endl;
    std::cout << styleFile.spriteBase.ped << std::endl;
    std::cout << unsigned(styleFile.getCarInfoSize()) << std::endl;

    // Load car sprites
    std::vector<QLabel*> carSprites;
    int j = 0;
    for (int i = 0; i < styleFile.getCarInfoSize(); i++) {
        QSprite sprite = QSprite(&styleFile);
        std::cout << j << std::endl;
        sprite.setSprite(j);
        QLabel *spriteLabel = new QLabel(this);
        spriteLabel->setPixmap(QPixmap::fromImage(sprite.getImage()));
        carSprites.push_back(spriteLabel);
        j += styleFile.getCarInfo(i).sprite;
    }
    sprites.push_back(carSprites);

    // Load other sprites
    for (int i = 1; i < 6; i++) {
        std::tuple<int, int> base_length = styleFile.getSpriteBase(i);
        std::cout << std::get<0>(base_length) << " - " << std::get<1>(base_length) << std::endl;
        sprites.push_back(LoadSprites(std::get<0>(base_length), std::get<0>(base_length)+std::get<1>(base_length)));
    }
}

std::vector<QLabel*> MainWindow::LoadSprites(int base, int nextBase) {
    std::vector<QLabel*> spriteVec;
    for (int i = base; i < nextBase; i++) {
        QSprite sprite = QSprite(&styleFile);
        sprite.setSprite(i);
        QLabel *spriteLabel = new QLabel(this);
        spriteLabel->setPixmap(QPixmap::fromImage(sprite.getImage()));
        spriteVec.push_back(spriteLabel);
    }
    return spriteVec;
}

void MainWindow::OnMenuCategoryCarsClick() {
    QList<QWidget*> children = scroll->findChildren<QWidget*>();
    qDebug() << children.size();
    /*for (int i = 0; i < sprites_visible.size(); i++) {
        if (sprites_visible[i]) {
            std::cout << i << std::endl;
            hlayout->removeWidget(sprites[i]);
            //scroll->layout()->removeWidget(sprites[i]);
        }
    }*/

    setSprites(0);
}

void MainWindow::setSprites(int category) {
    if (spritesActive != -1) {
        for (int i = 0; i < sprites[spritesActive].size(); i++) {
            std::cout << i << std::endl;
            hlayout1->removeWidget(sprites[spritesActive][i]);
            hlayout2->removeWidget(sprites[spritesActive][i]);
            hlayout3->removeWidget(sprites[spritesActive][i]);
            sprites[spritesActive][i]->setParent(nullptr);
            //scroll->layout()->removeWidget(sprites[i]);
        }
    }

    int perRow = sprites[category].size() / 3;
    for (int i = 0; i < perRow; i++) {
        hlayout1->addWidget(sprites[category][i]);
    }
    for (int i = perRow; i < perRow*2; i++) {
        hlayout2->addWidget(sprites[category][i]);
    }
    for (int i = perRow*2; i < perRow*3; i++) {
        hlayout3->addWidget(sprites[category][i]);
    }
    spritesActive = category;
}

void MainWindow::OnMenuCategoryPedsClick() {
    /*QList<QWidget*> children = scroll->findChildren<QWidget*>();
    qDebug() << children.size();
    for (int i = 0; i < carSprites.size(); i++) {
        std::cout << i << std::endl;
        hlayout2->removeWidget(carSprites[i]);
            //scroll->layout()->removeWidget(sprites[i]);
    }*/

    setSprites(1);
}

void MainWindow::OnMenuCategoryMapObjClick() {
    setSprites(2);
}

void MainWindow::OnMenuCategoryCodeObjClick() {
    setSprites(3);
}

void MainWindow::OnMenuCategoryUserClick() {
    setSprites(4);
}

void MainWindow::OnMenuCategoryFontsClick() {
    setSprites(5);
}
