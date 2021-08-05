//
// Created by Jon Lachmann on 2021-08-03.
//

#include "QSprite.h"

QSprite::QSprite(Style* style) : style(style) {}

QImage QSprite::getImage() {
    if (!rendered) renderImage();
    return this->image;
}

void QSprite::setSprite(int sprite, int type) {
    this->sprite = sprite;
    this->type = type;
    rendered = false;
}

void QSprite::renderImage() {
    physicalPalette = style->getSpritePalette(this->sprite, type, remap);
    spriteEntry = style->getSpriteIndex()[this->sprite];

    image = QImage(spriteEntry.w, spriteEntry.h, QImage::Format_RGB32);
    spriteData = style->getSingleSpriteData(sprite);
    QRgb value;
    for (int i = 0; i < spriteEntry.h; i++) {
        for (int j = 0; j < spriteEntry.w; j++) {
            value = qRgb(physicalPalette.colors[spriteData[i*spriteEntry.w+j]][2],
                         physicalPalette.colors[spriteData[i*spriteEntry.w+j]][1],
                         physicalPalette.colors[spriteData[i*spriteEntry.w+j]][0]);
            image.setPixel(j, i, value);
        }
    }
    rendered = true;
}

void QSprite::setRemap(int remap) {
    this->remap = remap;
    rendered = false;
}
