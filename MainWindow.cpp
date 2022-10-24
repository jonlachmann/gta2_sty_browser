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

    QMenu* menuActions = menuBar()->addMenu("&Actions");
    menuActions->addAction("&Increase remap", this, &MainWindow::OnMenuActionsIncreaseRemap, QKeySequence(Qt::CTRL + Qt::Key_Plus));
    menuActions->addAction("&Decrease remap", this, &MainWindow::OnMenuActionsDecreaseRemap, QKeySequence(Qt::CTRL + Qt::Key_Minus));

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

    LoadStyleFile(file1Name.toStdString());
}

void MainWindow::LoadStyleFile(std::string file) {
    clearSprites();
    styleFile.load(file, true);
    sprite_images.clear();
    sprites.clear();

    // Load car sprites
    std::vector<QLabel*> carSprites;
    std::vector<QSprite> spriteImageVec;
    int j = 0;
    for (int i = 0; i < styleFile.getCarInfoSize(); i++) {
        QSprite sprite = QSprite(&styleFile);
        std::cout << j << std::endl;
        sprite.setSprite(j, 0);
        QLabel *spriteLabel = new QLabel(this);
        spriteLabel->setPixmap(QPixmap::fromImage(sprite.getImage()));
        carSprites.push_back(spriteLabel);
        j += styleFile.getCarInfo(i).sprite;
        spriteImageVec.push_back(sprite);
    }
    sprite_images.push_back(spriteImageVec);
    sprites.push_back(carSprites);

    // Load other sprites
    for (int i = 1; i < 6; i++) {
        std::tuple<int, int> base_length = styleFile.getSpriteBase(i);
        LoadSprites(std::get<0>(base_length), std::get<0>(base_length)+std::get<1>(base_length), i);
    }
}

void MainWindow::test() {
    LoadStyleFile("../../openGTA2/data/ste.sty");
}

void MainWindow::LoadSprites(int base, int nextBase, int type) {
    std::vector<QLabel*> spriteVec;
    std::vector<QSprite> spriteImageVec;
    for (int i = base; i < nextBase; i++) {
        QSprite sprite = QSprite(&styleFile);
        sprite.setSprite(i, type);
        QLabel *spriteLabel = new QLabel(this);
        spriteLabel->setPixmap(QPixmap::fromImage(sprite.getImage()));
        spriteVec.push_back(spriteLabel);
        spriteImageVec.push_back(sprite);
    }
    sprite_images.push_back(spriteImageVec);
    sprites.push_back(spriteVec);
}

void MainWindow::clearSprites() {
    if (spritesActive != -1) {
        for (int i = 0; i < sprites[spritesActive].size(); i++) {
            hlayout1->removeWidget(sprites[spritesActive][i]);
            hlayout2->removeWidget(sprites[spritesActive][i]);
            hlayout3->removeWidget(sprites[spritesActive][i]);
            sprites[spritesActive][i]->setParent(nullptr);
            if (remap != -1) {
                sprite_images[spritesActive][i].setRemap(-1);
                sprites[spritesActive][i]->setPixmap(QPixmap::fromImage(sprite_images[spritesActive][i].getImage()));
            }
        }
    }
}

void MainWindow::setSprites(int category) {
    clearSprites();

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
    remap = -1;
}

void MainWindow::OnMenuCategoryCarsClick() {
    setSprites(0);
}

void MainWindow::OnMenuCategoryPedsClick() {
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

void MainWindow::OnMenuActionsIncreaseRemap() {
    remap++;
    std::cout << "Remap: " << remap << std::endl;
    for (int j = 0; j < sprites[spritesActive].size(); j++) {
        sprite_images[spritesActive][j].setRemap(remap);
        sprites[spritesActive][j]->setPixmap(QPixmap::fromImage(sprite_images[spritesActive][j].getImage()));
    }
}

void MainWindow::OnMenuActionsDecreaseRemap() {
    remap-=2;
    MainWindow::OnMenuActionsIncreaseRemap();
}
