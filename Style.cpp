//
// Created by Jon Lachmann on 2020-03-31.
//

#include "Style.h"

uint32_t Style::readChunkHeader(SDL_RWops* file, uint32_t &size) {
    uint32_t type;
    file->read(file, &type, 4, 1);
    file->read(file, &size, 4, 1);
    return type;
}

palette_index Style::readPaletteIndex(SDL_RWops* file) {
    palette_index palx;
    file->read(file, &palx, sizeof(palx), 1);
    return palx;
}

palette_base Style::readPaletteBase(SDL_RWops* file) {
    palette_base palb;
    file->read(file, &palb, sizeof(palb), 1);

    // Adjust indices to point at the first sprite of the type
    palb.font_remap = palb.user_remap + palb.map_obj_remap +  palb.ped_remap;
    palb.user_remap = palb.map_obj_remap + palb.code_obj_remap + palb.ped_remap + palb.car_remap + palb.sprite + palb.tile;
    palb.map_obj_remap = palb.code_obj_remap + palb.ped_remap + palb.car_remap + palb.sprite + palb.tile;
    palb.code_obj_remap = palb.ped_remap + palb.car_remap + palb.sprite + palb.tile;
    palb.ped_remap = palb.car_remap + palb.sprite + palb.tile;
    palb.car_remap = palb.sprite + palb.tile;
    palb.sprite = palb.tile;
    std::cout << unsigned(palb.font_remap) << " : " << unsigned(palb.tile) << std::endl;
    palb.tile = 0;
    return palb;
}

std::vector<physical_palette> Style::readPhysicalPalettes(SDL_RWops* file, uint32_t size) {
    uint8_t page[64*256][4];
    int pageCount = size / sizeof(page);
    std::vector<physical_palette> palettes;
    for (int p = 0; p < pageCount; p++) {
        file->read(file, &page, sizeof(page), 1);
        physical_palette palette;
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 256; j++) {
                for (int k = 0; k < 4; k++) {
                    palette.colors[j][k] = page[i+j*64][k];
                }
            }
            palettes.push_back(palette);
        }
    }
    return palettes;
}


std::vector<tile_data> Style::readTileData(SDL_RWops* file, uint32_t size) {
    std::vector<tile_data> tiles;
    tile_data tile1, tile2, tile3, tile4;
    int halfPageCount = size / (sizeof(tile_data) * 4);
    for (int i = 0; i < halfPageCount; i++) {
        int tileRows = 64;
        for (int row = 0; row < tileRows; row++) {
            file->read(file, &tile1.pixels[row], sizeof(tile1.pixels[row]), 1);
            file->read(file, &tile2.pixels[row], sizeof(tile2.pixels[row]), 1);
            file->read(file, &tile3.pixels[row], sizeof(tile3.pixels[row]), 1);
            file->read(file, &tile4.pixels[row], sizeof(tile4.pixels[row]), 1);
        }
        tiles.push_back(tile1);
        tiles.push_back(tile2);
        tiles.push_back(tile3);
        tiles.push_back(tile4);
    }
    return tiles;
}

std::vector<uint8_t> Style::readRawSprites(SDL_RWops* file, uint32_t size) {
    std::vector<uint8_t> sprg_vec(size);
    file->read(file, sprg_vec.data(), size, 1);

    return sprg_vec;
}

std::vector<sprite_data> Style::parseSpriteData(std::vector<uint8_t> sprg, std::vector<sprite_entry> spriteIndex) {
    std::vector<sprite_data> sprites;
    for (auto&& spriteData:spriteIndex) {
        sprite_data sprite;
        int width = spriteData.w + spriteData.w%4; //It is still a mystery why this works.
        for (int i = 0; i < spriteData.h; i++) {
            for (int j = 0; j < width; j++) {
                sprite.pixels.push_back(sprg[spriteData.ptr + j + i*256]);
            }
        }
        sprites.push_back(sprite);
    }
    return sprites;
}

font_base Style::readFontBase(SDL_RWops* file) {
    font_base fonb;
    file->read(file, &fonb.font_count, sizeof(fonb.font_count), 1);
    uint16_t last_base = 0;
    for (int i = 0; i < fonb.font_count; i++) {
        uint16_t base;
        file->read(file, &base, sizeof(base), 1);
        fonb.base.push_back(last_base);
        std::cout << "Font: " << unsigned(last_base) << "Base: " << unsigned(base) << std::endl;
        last_base += base;
    }
    return fonb;
}

sprite_base Style::readSpriteBase(SDL_RWops* file) {
    sprite_base sprb;
    file->read(file, &sprb, sizeof(sprb), 1);

    //std::cout << "cars:" << sprb.car << "peds:" << sprb.ped <<  "code_obj:" << sprb.code_obj << "map_obj:" << sprb.map_obj << "user:" << sprb.user << "font:" << sprb.font << std::endl;

    // Adjust indices to point at the first sprite of the type
    sprb.font = sprb.user + sprb.map_obj + sprb.code_obj + sprb.ped + sprb.car;
    sprb.user = sprb.map_obj + sprb.code_obj + sprb.ped + sprb.car;
    sprb.map_obj = sprb.code_obj + sprb.ped + sprb.car;
    sprb.code_obj = sprb.ped + sprb.car;
    sprb.ped = sprb.car;
    sprb.car = 0;

    return sprb;
}

std::vector<sprite_entry> Style::readSpriteIndex(SDL_RWops* file, uint32_t size) {
    std::vector<sprite_entry> sprx;
    int count = size / sizeof(sprite_entry);
    for (int i = 0; i < count; i++) {
        sprite_entry spre;
        file->read(file, &spre, sizeof(spre), 1);
        sprx.push_back(spre);
    }
    return sprx;
}

std::vector<delta> Style::readDeltaStore(SDL_RWops *file, uint32_t size) {
    SDL_RWops *outfile = SDL_RWFromFile("test.txt", "w");
    int printedChars = 0;
    int deltaLength = 0;

    std::cout << "size:" << unsigned(size) << std::endl;
    int startOffset = SDL_RWtell(file);
    std::vector<delta> dels;
    while (SDL_RWtell(file) < startOffset + size) {
        delta del;
        file->read(file, &del.offset, sizeof(del.offset), 1);
        file->read(file, &del.length, sizeof(del.length), 1);
        for (int i = 0; i < del.length; i++) {
            uint8_t delta_data;
            file->read(file, &delta_data, sizeof(delta_data), 1);
            del.data.push_back(delta_data);
        }
        //std::cout << "Delta len: " << unsigned(del.length) << " offset: " << unsigned(del.offset) << std::endl;

        deltaLength += 3+del.length;
        for (int i=0; i < del.offset; i++) {
            SDL_RWwrite(outfile, " ", 1, 1);
            printedChars++;
            if (printedChars == 256){
                SDL_RWwrite(outfile, "\n", 1, 1);
                SDL_RWwrite(outfile, std::to_string(deltaLength).data(), std::to_string(deltaLength).length(), 1);
                printedChars = 0;
            }
            //std::cout << " ";
        }
        for (int i=0; i < del.length; i++) {
            SDL_RWwrite(outfile, "x", 1, 1);
            printedChars++;
            if (printedChars == 256){
                SDL_RWwrite(outfile, "\n", 1, 1);
                SDL_RWwrite(outfile, std::to_string(deltaLength).data(), std::to_string(deltaLength).length(), 1);
                printedChars = 0;
            }
            //std::cout << "x";
        }
        dels.push_back(del);
    }
    SDL_RWclose(outfile);
    return dels;
}

std::vector<delta_entry> Style::readDeltaIndex(SDL_RWops* file, uint32_t size) {
    int startOffset = SDL_RWtell(file);
    std::vector<delta_entry> delx;
    while (SDL_RWtell(file) < startOffset + size) {
        delta_entry dele;
        file->read(file, &dele.which_sprite, sizeof(dele.which_sprite), 1);
        file->read(file, &dele.delta_count, sizeof(dele.delta_count), 1);
        file->read(file, &dele.pad, sizeof(dele.pad), 1);
        for (int i = 0; i < dele.delta_count; i++) {
            uint16_t delta_size;
            file->read(file, &delta_size, sizeof(delta_size), 1);
            dele.delta_size.push_back(delta_size);
            //std::cout << "Delta sz:" << unsigned(delta_size) << " sprite:" << unsigned(dele.which_sprite) << std::endl;
        }
        delx.push_back(dele);
    }
    return delx;
}

std::vector<object_info> Style::readObjectInfo(SDL_RWops* file, uint32_t size) {
    std::vector<object_info> obji;
    int count = size / sizeof(object_info);
    for (int i = 0; i < count; i++) {
        object_info obj;
        file->read(file, &obj, sizeof(obj), 1);
        obji.push_back(obj);
    }
    return obji;
}

std::vector<car_info> Style::readCarInfo(SDL_RWops* file, uint32_t size) {
    int startOffset = SDL_RWtell(file);
    std::vector<car_info> cari_vec;
    while (SDL_RWtell(file) < startOffset + size) {
        car_info cari;
        file->read(file, &cari.model, sizeof(cari.model), 1);
        file->read(file, &cari.sprite, sizeof(cari.sprite), 1);
        file->read(file, &cari.w, sizeof(cari.w), 1);
        file->read(file, &cari.h, sizeof(cari.h), 1);
        file->read(file, &cari.num_remaps, sizeof(cari.num_remaps), 1);
        file->read(file, &cari.passengers, sizeof(cari.passengers), 1);
        file->read(file, &cari.wreck, sizeof(cari.wreck), 1);
        file->read(file, &cari.rating, sizeof(cari.rating), 1);
        file->read(file, &cari.front_wheel_offset, sizeof(cari.front_wheel_offset), 1);
        file->read(file, &cari.rear_wheel_offset, sizeof(cari.rear_wheel_offset), 1);
        file->read(file, &cari.front_window_offset, sizeof(cari.front_window_offset), 1);
        file->read(file, &cari.rear_window_offset, sizeof(cari.rear_window_offset), 1);
        file->read(file, &cari.info_flags, sizeof(cari.info_flags), 1);
        file->read(file, &cari.info_flags_2, sizeof(cari.info_flags_2), 1);
        for (int i = 0; i < cari.num_remaps; i++) {
            uint8_t remap;
            file->read(file, &remap, sizeof(remap), 1);
            cari.remap.push_back(remap);
        }
        file->read(file, &cari.num_doors, sizeof(cari.num_doors), 1);
        for (int i = 0; i < cari.num_doors; i++) {
            door_info door;
            file->read(file, &door, sizeof(door), 1);
            cari.doors.push_back(door);
        }
        cari_vec.push_back(cari);
    }
    return cari_vec;
}

std::vector<SDL_physical_palette> Style::convertPalettes(std::vector<physical_palette> physicalPalettes) {
    std::vector<SDL_physical_palette> sdlPalettes;
    for (auto&& palette:physicalPalettes) {
        SDL_physical_palette sdlPalette;
        for (int i=0; i < 256; i++) {
            sdlPalette.colors[i].b = palette.colors[i][0];
            sdlPalette.colors[i].g = palette.colors[i][1];
            sdlPalette.colors[i].r = palette.colors[i][2];
            sdlPalette.colors[i].a = palette.colors[i][3];
        }
        sdlPalettes.push_back(sdlPalette);
    }
    return sdlPalettes;
}

SDL_Surface* Style::getTileAtlas() {
    uint32_t rmask,gmask,bmask,amask;
    bmask = 0x000000ff;
    gmask = 0x0000ff00;
    rmask = 0x00ff0000;
    amask = 0xff000000;

    SDL_Surface* surf = SDL_CreateRGBSurface(0,2048,2048,32,rmask,gmask,bmask,amask);
    static uint32_t atlas[2048][2048];
    for (int tile = 0; tile < tileData.size(); tile++) {
        int palette = paletteIndex.phys_palette[tile];
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 64; j++) {
                uint32_t *pixel;
                pixel = (uint32_t*)physicalPalettes[palette].colors[tileData[tile].pixels[i][j]];
                uint32_t alpha = ((tileData[tile].pixels[i][j] > 0) * 0xff000000);
                atlas[i+(tile/32)*64][j + (tile%32)*64] = (*pixel) + alpha;
            }
        }
    }
    surf->pixels = atlas;
    return surf;
}

void Style::genDeltaSprites() {
    int storeIndex = 0;
    for (int entry=0; entry < deltaIndex.size(); entry++) {
        for (int delta=0; delta < deltaIndex[entry].delta_count; delta++) {
            delta_sprite deltaSprite;
            deltaSprite.which_sprite = deltaIndex[entry].which_sprite;
            deltaSprite.w = spriteIndex[deltaIndex[entry].which_sprite].w;
            deltaSprite.h = spriteIndex[deltaIndex[entry].which_sprite].h;
            deltaSprite.data = std::vector<uint16_t>(deltaSprite.w*deltaSprite.h, 256);

            int offset = 0;
            int position = 0;
            while (position < deltaIndex[entry].delta_size[delta]) {
                offset += deltaStore[storeIndex].offset;
                int y = offset / 256;
                int x = offset % 256;
                for (int i=0; i < deltaStore[storeIndex].length; i++) {
                    deltaSprite.data[y*deltaSprite.w+x+i] = deltaStore[storeIndex].data[i];
                }
                offset += deltaStore[storeIndex].length;
                position += 3 + deltaStore[storeIndex].length;
                storeIndex++;
            }
            deltaSprites.push_back(deltaSprite);
        }
    }
}

void Style::genDeltaAtlas(int atlasWidth) {
    int maxY = deltaSprites[0].h;
    deltaAtlas.resize(atlasWidth*maxY);

    int nextX = 0;
    int nextY = 0;
    for (int i=0; i < deltaSprites.size(); i++) {
        // If row is full, make a new one
        if (deltaSprites[i].w > atlasWidth-nextX){
            nextY += maxY;
            nextX = 0;
            maxY = deltaSprites[i].h;
            deltaAtlas.resize(atlasWidth*(maxY+nextY));
        }
        // Make row higher if necessary
        if (deltaSprites[i].h > maxY) {
            maxY = deltaSprites[i].h;
            deltaAtlas.resize(atlasWidth*(maxY+nextY));
        }
        // Put the deltasprite in the atlas, and save its index
        delta_atlas_entry deltaIndex;
        deltaIndex.w = deltaSprites[i].w;
        deltaIndex.h = deltaSprites[i].h;
        deltaIndex.ptr = nextY*atlasWidth+nextX;
        deltaIndex.which_sprite = deltaSprites[i].which_sprite;
        if (i < 20) deltaAtlasIndex.push_back(deltaIndex);

        for (int y=0; y < deltaSprites[i].h; y++) {
            for (int x=0; x < deltaSprites[i].w; x++) {
                deltaAtlas[(nextY+y)*atlasWidth+nextX+x] = deltaSprites[i].data[deltaSprites[i].w*y+x];
            }
        }
        nextX += deltaSprites[i].w;
    }
    /*std::vector<uint8_t> deltaAtlas8;
    deltaAtlas8.resize(deltaAtlas.size());
    for (int i=0; i < deltaAtlas.size(); i++) deltaAtlas8[i] = deltaAtlas[i];
    SDL_Surface* surf = SDL_CreateRGBSurface(0,atlasWidth,nextY+maxY,8,0,0,0,0);
    SDL_SetPaletteColors(surf->format->palette, sdlPhysicalPalettes[30].colors, 0, 255);

    surf->pixels = deltaAtlas8.data();

    SDL_SaveBMP(surf, "deltas.bmp");*/
}

int Style::drawSpritePage(int page, char filename[]) {
    int pageCount = rawSprites.size()/(256*256);

    SDL_Surface *pageSurface = SDL_CreateRGBSurface(0, 256, pageCount*256, 8, 0, 0, 0, 0);

    SDL_SetPaletteColors(pageSurface->format->palette, sdlPhysicalPalettes[8].colors, 0, 255);

    pageSurface->pixels = rawSprites.data();

    SDL_SaveBMP(pageSurface, filename);

    return 0;

}

int Style::drawPalettes(char filename[]) {
    int paletteCount = physicalPalettes.size();

    SDL_Surface *palSurface = SDL_CreateRGBSurface(0, 256, paletteCount, 32, 0, 0, 0, 0);

    //SDL_SetPaletteColors(pageSurface->format->palette, sdlPhysicalPalettes[30].colors, 0, 255);

    palSurface->pixels = physicalPalettes.data();

    SDL_SaveBMP(palSurface, filename);

    return 0;

}



Style::Style() {}

void Style::load(std::string filename, bool verbose) {
    SDL_RWops* file = SDL_RWFromFile(filename.data(), "rb");

    // Read the header and save it
    file->read(file, header, sizeof(header), 1);
    file->read(file, &version, sizeof(version), 1);

    if (verbose) std::cout << header[0] << header[1] << header[2] << header[3] << ", " << version << std::endl;

    bool deltas = false;

    // Read file and save data in correct structures
    int chunk = 0;
    while (chunk < 15) {
        uint32_t size = 0;
        uint32_t type = readChunkHeader(file, size);
        switch (type) {
            case PALX:
                paletteIndex = readPaletteIndex(file);
                if (verbose) std::cout << "Read PALX" << std::endl;
                break;
            case PPAL:
                physicalPalettes = readPhysicalPalettes(file, size);
                if (verbose) std::cout << "Read PPAL, palette count: " << physicalPalettes.size() << std::endl;
                break;
            case PALB:
                paletteBase = readPaletteBase(file);
                if (verbose) std::cout << "Read PALB" << std::endl;
                break;
            case TILE:
                tileData = readTileData(file, size);
                if (verbose) std::cout << "Read TILE, tile count: " << tileData.size() << std::endl;
                break;
            case SPRB:
                spriteBase = readSpriteBase(file);
                if (verbose) std::cout << "Read SPRB" << std::endl;
                break;
            case SPRX:
                spriteIndex = readSpriteIndex(file, size);
                if (verbose) std::cout << "Read SPRX, sprite count: " << spriteIndex.size() << std::endl;
                break;
            case SPRG:
                rawSprites = readRawSprites(file, size);
                if (verbose) std::cout << "Read raw SPRG" << std::endl;
                break;
            case DELS:
                deltaStore = readDeltaStore(file, size);
                if (verbose) std::cout << "Read DELS, delta count: " << deltaStore.size() << std::endl;
                deltas = true;
                break;
            case DELX:
                deltaIndex = readDeltaIndex(file, size);
                if (verbose) std::cout << "Read DELX, delta count: " << deltaIndex.size() << std::endl;
                break;
            case FONB:
                fontBase = readFontBase(file);
                if (verbose) std::cout << "Read FONB, font count: " << unsigned(fontBase.font_count) << std::endl;
                break;
            case CARI:
                carInfo = readCarInfo(file, size);
                if (verbose) std::cout << "Read CARI, car count: " << carInfo.size() << std::endl;
                //for (auto&& car:carInfo) std::cout << "Car size " << unsigned(car.w) << "*" << unsigned(car.h) << ", passengers: " << unsigned(car.passengers) << std::endl;
                break;
            case OBJI:
                objectInfo = readObjectInfo(file, size);
                if (verbose) std::cout << "Read OBJI, object count: " << objectInfo.size() << std::endl;
                break;
            default:
                SDL_RWseek(file, size, RW_SEEK_CUR);
                if (verbose) std::cout << "Unknown chunk, skipping" << std::endl;
        }
        chunk++;
    }

    // Reading file completed, prepare data to be used
    spriteGraphics = parseSpriteData(rawSprites, spriteIndex);
    sdlPhysicalPalettes = convertPalettes(physicalPalettes);

    if (deltas) {
        genDeltaSprites();
        genDeltaAtlas(2048);
    }
}

std::vector<uint8_t> Style::getRawSprites() {
    return rawSprites;
}

const std::vector<physical_palette> Style::getPhysicalPalettes() const {
    return physicalPalettes;
}

std::vector<sprite_entry> Style::getSpriteIndex() {
    return spriteIndex;
}

std::vector<uint16_t> Style::getDeltaAtlas() {
    return deltaAtlas;
}

std::vector<delta_atlas_entry> Style::getDeltaAtlasIndex() {
    return deltaAtlasIndex;
}

int Style::getFontSprite(char character, int font) {
    // -33 because the fonts start at ASCII 33 = !
    character -= 33;
    // Check if the font exists
    if (fontBase.font_count > font) {
        return spriteBase.font + character + fontBase.base[font];
    }
    // Font does not exist, return the first character of the first font
    return spriteBase.font;
}

int Style::getSpriteWidth(int sprite) {
    return spriteIndex[sprite].w;
}
int Style::getSpriteHeight(int sprite) {
    return spriteIndex[sprite].h;
}

int Style::getCarSpriteId(int car) {
    // Return car sprite
    return spriteBase.car + car;
}

int Style::getPedSpriteId(int skin, int sprite) {
    // Return ped sprite, each skin has 158 sprites
    return spriteBase.ped + skin * 158 + sprite;
}

std::vector<uint8_t> Style::getSingleSpriteData(int sprite) {
    std::vector<uint8_t> spriteData;
    sprite_entry spriteInfo = spriteIndex[sprite];


    for (int i = 0; i < spriteInfo.h; i++) {
        for (int j = 0; j < spriteInfo.w; j++) {
            spriteData.push_back(rawSprites[spriteInfo.ptr + i*256 + j]);
        }
    }

    return spriteData;
}

physical_palette Style::getSpritePalette(int sprite, int type, int remap) {
    int virtualPalette = sprite + paletteBase.sprite + remap;

    return physicalPalettes[paletteIndex.phys_palette[virtualPalette]];
}
