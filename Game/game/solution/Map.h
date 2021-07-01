//
// Created by Дмитрий Калашников on 21.02.2021.
//

#pragma once
#ifndef MAIN_MAP_H
#define MAIN_MAP_H

#include "Image.h"
#include "Tile.h"

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

struct MapPoint
{
    char room_type;
    int room_x = 0;
    int room_y = 0;
};


struct Map
{

    bool changed = true;
    int dimming = 0;
    static constexpr int MAX_DIMING = 5;
    double dimming_coef = 1.0 / MAX_DIMING;
    bool goes_down = true;

    MapPoint player_start;
    std::vector<MapPoint> illum;
    std::vector<MapPoint> enemies;

    Map() : changed(true) {}
    ~Map();

    int Height() const { return map_height; }
    int Width() const { return map_width; }
    int get_room_size() const { return RoomSize; }

    int &get_map_x() { return map_x; }
    int &get_map_y() { return map_y; }
    std::vector<std::string> &get_map_plan() { return map_plan; }

    bool load_map(const std::string &base_dir, const std::string &map_path);

    bool set_pixel_map( Tile &tiles);

    void Draw(Image &screen);

    bool initialize(Image &screen);

    void scan();

    bool add_background(Image &bg_image);

    void blur_map(int r);
    void set_illumination();

    Pixel GetPixel(int x, int y);
    Pixel GetPixel(char room_type, int x, int y);
    void PutPixel(int x, int y, const Pixel &pix);

    char current_room_type() { return map_plan[map_x][map_y]; }
    void set_start_map(int x, int y) { start_map_x = x; start_map_y = y; }
    void reset_map_coords() { map_x = start_map_x; map_y = start_map_y; }

    std::vector<std::string> &get_data() { return rooms_plan[ map_plan[map_x][map_y] ]; }

private:

    std::unordered_map<char, std::vector<std::string>> rooms_plan;
    std::unordered_map<char, Image *> pixel_rooms_map;
    std::vector<std::string> map_plan;

    int map_width = 0;
    int map_height = 0;
    int RoomSize = 0;

    int map_x = 0;
    int map_y = 0;
    int start_map_x = 0;
    int start_map_y = 0;

    bool set_map_plan(const std::string &base_dir,
                      const std::string &map_path);
    bool get_rooms_plan(const std::string &base_dir);
    std::string get_prefix(char c);

    void map_print();
};


#endif //MAIN_MAP_H
