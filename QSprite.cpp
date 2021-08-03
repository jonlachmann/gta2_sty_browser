//
// Created by Jon Lachmann on 2021-08-03.
//

#include "QSprite.h"

QSprite::QSprite(Style* style) : style(style) {}

QImage QSprite::getImage() {
    if (!rendered) renderImage();
    return this->image;
}

void QSprite::setPalette(int palette) {
    this->palette = palette;
    rendered = false;
}

void QSprite::setSprite(int sprite) {
    this->sprite = sprite;
    rendered = false;
}

void QSprite::renderImage() {
    physicalPalette = style->getSpritePalette(this->palette,1,0);
    spriteEntry = style->getSpriteIndex()[this->sprite];

    image = QImage(spriteEntry.w, spriteEntry.h, QImage::Format_RGB32);
    spriteData = style->getSingleSpriteData(sprite);
    QRgb value;
    for (int i = 0; i < spriteEntry.h; i++) {
        for (int j = 0; j < spriteEntry.w; j++) {
            value = qRgb(physicalPalette.colors[spriteData[i*spriteEntry.w+j]][0],
                         physicalPalette.colors[spriteData[i*spriteEntry.w+j]][1],
                         physicalPalette.colors[spriteData[i*spriteEntry.w+j]][2]);
            image.setPixel(j, i, value);
        }
    }
}
