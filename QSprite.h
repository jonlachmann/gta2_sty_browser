//
// Created by Jon Lachmann on 2021-08-03.
//

#ifndef STY_BROWSER_QSPRITE_H
#define STY_BROWSER_QSPRITE_H


#include <QImage>
#include "Style.h"

class QSprite {

private:
    Style* style;
    sprite_entry spriteEntry;
    physical_palette physicalPalette;

    QImage image;
    std::vector<uint8_t> spriteData;
    int rendered = false;

    int sprite = 0;
    int type;
    int remap = -1;

    void renderImage();

public:
    QSprite(Style *style);

    void setStyle(Style* style);
    QImage getImage();

    void setSprite(int sprite, int type);

    void setRemap(int remap);

};


#endif //STY_BROWSER_QSPRITE_H
