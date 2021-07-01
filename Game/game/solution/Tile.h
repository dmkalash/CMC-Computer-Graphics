#pragma once
#ifndef MAIN_TILE_H
#define MAIN_TILE_H

#include "Image.h"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>


struct Tile
{
    std::unordered_map<char, Image *> tile_dict;

    Tile(const std::unordered_map<std::string, char> &n2s = {},
            const std::vector<std::string> &names = {}) {
        if (!n2s.empty() && !names.empty()) {
            name_to_symb = n2s;
            this->names = names;
        }
    }

    ~Tile();

    bool load_tiles(const std::string &base_dir, const std::vector<std::string> &names);
    bool fix_tileset(const std::string &base_dir);


    std::unordered_map<std::string, char> name_to_symb = {
            {"point", '.'},
            {"at", '@'},
            {"sharp", '#'},
            {"blank", ' '},
            {"x", 'x'},
            {"Q", 'Q'},
            {"T", 'T'},
            {"S", 'S'},
            {"player", 'p'},
            {"enemy", 'E'},
            {"I", 'I'},
    };
    std::vector<std::string> names = {
            "point_tile.png",
            "at_tile.png",
            "sharp_tile.png",
            "blank_tile.png",
            "x_tile.png",
            "Q_tile.png",
            "T_tile.png",
            "S_tile.png",
            "player_tile.png",
            "enemy_tile.png",
            "I_tile.png"
    };

private:

    bool get_pref(const std::string &s, std::string &pref);
};


struct Sprite {
    std::vector<Image *> tiles;
    int step = 0;
    int step_count = 0;
    int repeat = 1;
    int tile_size = -1;
    bool is_blur = false;

    Sprite() {}
    void initialize(bool is_blur, int repeat, const std::string &pref);
    void update_step() { step = ( step + 1 ) % (step_count * repeat); }
    int get_step() { return step / step_count; };

    ~Sprite();
};


#endif //MAIN_TILE_H
