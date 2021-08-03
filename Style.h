//
// Created by Jon Lachmann on 2020-03-31.
//

#ifndef OPENGTA2_STYLE_H
#define OPENGTA2_STYLE_H

#include <iostream>
#include <SDL.h>
#include <iomanip>
#include <vector>

struct tile_data {
    uint8_t pixels[64][64];
};

struct physical_palette {
    uint8_t colors[256][4];
};

struct SDL_physical_palette {
    SDL_Color colors[256];
};

struct sprite_data {
    std::vector<uint8_t> pixels;
};

struct palette_index {
    uint16_t phys_palette[16384];
};

struct palette_base {
    uint16_t tile;
    uint16_t sprite;
    uint16_t car_remap;
    uint16_t ped_remap;
    uint16_t code_obj_remap;
    uint16_t map_obj_remap;
    uint16_t user_remap;
    uint16_t font_remap;
};

struct sprite_base {
    uint16_t car;
    uint16_t ped;
    uint16_t code_obj;
    uint16_t map_obj;
    uint16_t user;
    uint16_t font;
};

struct sprite_entry {
    uint32_t ptr;
    uint8_t w,h;
    uint16_t pad;
};

struct delta {
    uint16_t offset;
    uint8_t length;
    std::vector<uint8_t> data;
};

struct delta_sprite {
    uint16_t which_sprite;
    uint8_t w,h;
    std::vector<uint16_t> data;
};

struct delta_entry {
    uint16_t which_sprite;
    uint8_t delta_count;
    uint8_t pad;
    std::vector<uint16_t> delta_size;
};

struct delta_atlas_entry {
    int ptr;
    uint8_t w,h;
    uint16_t which_sprite;
};

struct door_info {
    int8_t rx, ry;
};

struct car_info {
    uint8_t model;
    uint8_t sprite;
    uint8_t w,h;
    uint8_t num_remaps;
    uint8_t passengers;
    uint8_t wreck;
    uint8_t rating;
    int8_t front_wheel_offset, rear_wheel_offset;
    int8_t front_window_offset, rear_window_offset;
    uint8_t info_flags;
    uint8_t info_flags_2;
    std::vector<uint8_t> remap;
    uint8_t num_doors;
    std::vector<door_info> doors;
};

struct font_base {
    uint16_t font_count;
    std::vector<uint16_t> base;
};

struct object_info {
    uint8_t model;
    uint8_t sprites;
};


class Style {
#define PALX 1481392464
#define PPAL 1279348816
#define PALB 1112293712
#define TILE 1162627412
#define SPRG 1196576851
#define SPRX 1481789523
#define SPRB 1112690771
#define DELS 1397507396
#define DELX 1481393476
#define FONB 1112428358
#define CARI 1230127427
#define OBJI 1229603407
#define RECY 1497580882
#define SPEC 1128616019


private:
    char header[4];
    uint16_t version;

    std::vector<sprite_entry> spriteIndex;

private:
    std::vector<tile_data> tileData;
    std::vector<delta_entry> deltaIndex;
    std::vector<delta> deltaStore;
    std::vector<car_info> carInfo;
    std::vector<object_info> objectInfo;
    font_base fontBase;

    std::vector<delta_atlas_entry> deltaAtlasIndex;
    std::vector<delta_sprite> deltaSprites;
    std::vector<uint16_t> deltaAtlas;

public:
    palette_index paletteIndex;
    sprite_base spriteBase;
    palette_base paletteBase;

private:
    std::vector<physical_palette> physicalPalettes;
public:
    const std::vector<physical_palette> getPhysicalPalettes() const;

private:
    std::vector<uint8_t> rawSprites;

private:
    std::vector<sprite_data> spriteGraphics;
    std::vector<SDL_physical_palette> sdlPhysicalPalettes;

    uint32_t readChunkHeader(SDL_RWops* file, uint32_t &size);
    palette_index readPaletteIndex(SDL_RWops* file);
    palette_base readPaletteBase(SDL_RWops* file);
    std::vector<physical_palette> readPhysicalPalettes(SDL_RWops* file, uint32_t size);
    std::vector<tile_data> readTileData(SDL_RWops* file, uint32_t size);
    std::vector<uint8_t> readRawSprites(SDL_RWops* file, uint32_t size);
    std::vector<sprite_data> parseSpriteData(std::vector<uint8_t> sprg, std::vector<sprite_entry> spriteIndex);
    font_base readFontBase(SDL_RWops* file);
    sprite_base readSpriteBase(SDL_RWops* file);
    std::vector<sprite_entry> readSpriteIndex(SDL_RWops* file, uint32_t size);
    std::vector<delta_entry> readDeltaIndex(SDL_RWops* file, uint32_t size);
    std::vector<delta> readDeltaStore(SDL_RWops* file, uint32_t size);
    std::vector<object_info> readObjectInfo(SDL_RWops* file, uint32_t size);
    std::vector<car_info> readCarInfo(SDL_RWops* file, uint32_t size);
    std::vector<SDL_physical_palette> convertPalettes(std::vector<physical_palette> physicalPalettes);

    void genDeltaSprites();
    void genDeltaAtlas(int atlasWidth);

public:
    Style();
    void load(std::string filename, bool verbose);

    SDL_Surface* getTileAtlas();

    std::vector<uint8_t> getRawSprites();
    std::vector<uint16_t> getDeltaAtlas();

    std::vector<sprite_entry> getSpriteIndex();
    std::vector<delta_atlas_entry> getDeltaAtlasIndex();

    int getFontSprite(char character, int font);
    int getCarSpriteId(int car);
    int getPedSpriteId(int skin, int sprite);

    std::vector<uint8_t> getSingleSpriteData(int sprite);

    physical_palette getSpritePalette(int sprite, int type, int remap);

    int getSpriteWidth(int sprite);
    int getSpriteHeight(int sprite);

    int drawSpritePage(int page, char filename[]);
    int drawPalettes(char filename[]);

};


#endif //OPENGTA2_STYLE_H
