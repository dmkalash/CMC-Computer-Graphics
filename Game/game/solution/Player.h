#pragma once
#ifndef MAIN_PLAYER_H
#define MAIN_PLAYER_H

#include "Image.h"
#include "Map.h"

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

struct Point
{
  int x = 0;
  int y = 0;
};

enum class MovementDir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

enum class Status
{
    GAME,
    MOVE,
    LOSS,
    WIN,
    OVER,
    RESTART
};

std::pair<Point, Point> get_room_coords(int x, int y);


static Point get_center(const Point &big_corner, const Point &small_corner)
{
    return Point {std::max( 0, big_corner.x / 2 - small_corner.x / 2 ),
                  std::max( 0, big_corner.y / 2 - small_corner.y / 2) };
}


struct Player
{
    Point coords {.x = 0, .y = 0};
    Point old_coords {.x = 0, .y = 0};
    Point start_coords {.x = 0, .y = 0};

    explicit Player(Point pos = {.x = 0, .y = 0}) :
            coords(pos), old_coords(coords), map() {};

    void set_current_status(const Status &s) { status = s; }
    Map &get_map() { return map; }

    bool Moved() const;
    void ProcessInput(MovementDir dir);

    void prepare_for_game(bool is_blur, int repeat);
    void reset_game();

    std::unordered_set<char> get_corners(int x, int y);
    bool can_move_next(Point p1, Point p2);
    Status get_current_status();

    void Draw(Image &screen);

private:

    Map map;
    Sprite sprite;
    Status status = Status::GAME;

    int player_tile_size;
    int move_speed = 4;

    std::unordered_set<char> valid_for_step = {'.', '@', 'x', 'T', 'Q'};

    void draw_main_game(Image &screen);
    void draw_over_game(Image &screen, const std::string &name);
    void draw_switching(Image &screen);
    void draw_empty_map(Image &screen);

    void update_current_room_and_position(Image &screen);

};


struct Enemy
{
    explicit Enemy(Point pos = {.x = tileSize, .y = tileSize}) : coords(pos),
                                                start_coords(pos),
                                                room_type('E') {};

    void prepare_for_game(bool is_blur, int repeat, Map &map);

    bool can_move_next(Player &player, int x, int y);

    void Draw(Player &player, Image &screen);

private:

    Sprite sprite;
    Point coords {.x = 0, .y = 0};
    Point start_coords {.x = 0, .y = 0};

    std::unordered_set<char> valid_for_step = {'.', '@', 'x', 'T', 'Q'};

    int move_speed = 2;
    int enemy_tile_size = -1;

    char room_type = 'E';






};

#endif //MAIN_PLAYER_H
