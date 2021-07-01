#include "Player.h"


std::pair<Point, Point> get_room_coords(int x, int y)
{
    return  {
            { .x = x / tileSize, .y = y / tileSize },
            { .x = (x + tileSize - 1) / tileSize, .y = (y + tileSize - 1) / tileSize }
    };
}


bool Player::Moved() const
{
    return !(coords.x == old_coords.x && coords.y == old_coords.y);
}


void Player::ProcessInput(MovementDir dir)
{
    int move_dist = move_speed * 1;

    if (status == Status::GAME) {
        switch(dir)
        {
            case MovementDir::UP:
                old_coords.y = coords.y;
                if ( can_move_next( Point {coords.x, coords.y - move_dist},
                                    Point {coords.x + player_tile_size - 1, coords.y - move_dist} ) ) {
                    coords.y -= move_dist;
                }
                break;
            case MovementDir::DOWN:
                old_coords.y = coords.y;

                if ( can_move_next( Point {coords.x, coords.y + player_tile_size + move_dist - 1},
                                    Point {coords.x + player_tile_size - 1,
                                           coords.y + player_tile_size + move_dist - 1} ) ) {
                    coords.y += move_dist;
                }
                break;
            case MovementDir::LEFT:
                old_coords.x = coords.x;
                if ( can_move_next( Point {coords.x - move_dist, coords.y},
                                    Point {coords.x - move_dist, coords.y + player_tile_size - 1} ) ) {
                    coords.x -= move_dist;
                }
                break;
            case MovementDir::RIGHT:
                old_coords.x = coords.x;
                if ( can_move_next( Point {coords.x + player_tile_size + move_dist - 1, coords.y},
                                    Point {coords.x + player_tile_size + move_dist - 1,
                                           coords.y + player_tile_size - 1} ) ) {
                    coords.x += move_dist;
                }
                break;
            default:
                break;
        }

        status = get_current_status();
    }
}


void Player::prepare_for_game(bool is_blur, int repeat)
{
    sprite.initialize(is_blur, repeat, "p");
    player_tile_size = sprite.tile_size;

    auto &symb_map = get_map().get_map_plan();
    char start_room_type = get_map().player_start.room_type;

    for (int y = 0; y < symb_map.size(); y++) {
        for (int x = 0; x < symb_map[0].size(); x++) {
            if (symb_map[y][x] == start_room_type) {
                get_map().get_map_x() = y;
                get_map().get_map_y() = x;
                get_map().set_start_map(y, x);
                y = symb_map.size(); break;
            }
        }
    }

    start_coords = old_coords = coords = { get_map().player_start.room_x * tileSize,
                                           get_map().player_start.room_y * tileSize };

    status = Status::GAME;
}


void Player::reset_game()
{
    if (status == Status::OVER || status == Status::LOSS || status == Status::WIN) {
        coords = old_coords = start_coords;
        status = Status::RESTART;
        get_map().reset_map_coords();
        sprite.step = 0;
    }
}


std::unordered_set<char> Player::get_corners(int x, int y)
{
    auto p = get_room_coords(x, y);

    int lx = p.first.x, rx = p.second.x;
    int ly = p.first.y, ry = p.second.y;

    char symb1 = map.get_data()[ly][lx];
    char symb2 = map.get_data()[ry][rx];
    char symb3 = map.get_data()[ry][lx];
    char symb4 = map.get_data()[ly][rx];

    std::unordered_set<char> corners = {symb1, symb2, symb3, symb4};
    return corners;
}


bool Enemy::can_move_next(Player &player, int x, int y)
{
    auto corners = player.get_corners(x, y);
    return !(corners.find('#') != corners.end() ||
             corners.find('Q') != corners.end() ||
             corners.find('x') != corners.end());
}


Status Player::get_current_status()
{

    auto corners = get_corners(coords.x, coords.y);

    if (corners.find('T') != corners.end()) {
        return Status::LOSS;
    } else if (corners.find('Q') != corners.end()) {
        return Status::WIN;
    } else if (corners.find('x') != corners.end()) {
        return Status::MOVE;
    } else {
        return Status::GAME;
    }
}


void Player::update_current_room_and_position(Image &screen) {
    Point lt { coords.x / tileSize, coords.y / tileSize };
    Point rb {(coords.x + player_tile_size) / tileSize, (coords.y + player_tile_size) / tileSize };

    int symb_x = lt.x;
    int symb_y = lt.y;
    int x_bias = 0, y_bias = 0;

    if (rb.x == get_map().get_room_size() - 1) {
        symb_x = 1;
        x_bias = 1;
    } else if (lt.x == 0) {
        symb_x = get_map().get_room_size() - 2;
        x_bias = -1;
    } else if (rb.y == get_map().get_room_size() - 1) {
        symb_y = 1;
        y_bias = 1;
    } else if (lt.y == 0) {
        symb_y = get_map().get_room_size() - 2;
        y_bias = -1;
    } else {
        std::cout << lt.x << " " << lt.y << " " << rb.x << " " << rb.y << std::endl;
        std::cout << "update_current_room_and_position: smth went wrong" << std::endl;
    }

    get_map().get_map_x() += y_bias;
    get_map().get_map_y() += x_bias;

    coords.x = symb_x * tileSize;
    coords.y = symb_y * tileSize;
    old_coords = coords;

    sprite.step = 0;
}





void Player::Draw(Image &screen)
{
    if (status == Status::MOVE) {
        draw_switching(screen);
    } else if (status == Status::WIN) {
        draw_over_game(screen, "win");
    } else if (status == Status::LOSS) {
        draw_over_game(screen, "lose");
    } else if (status == Status::GAME) {
        draw_main_game(screen);
    } else if (status == Status::RESTART) {
        get_map().Draw(screen);
        draw_main_game(screen);
        status = Status::GAME;
    }
}


void Player::draw_switching(Image &screen)
{
    if (get_map().goes_down && get_map().dimming < get_map().MAX_DIMING) {
        ++get_map().dimming;
        get_map().Draw(screen);
        return;
    }

    Point new_coords;
    if ( get_map().goes_down ) {
        get_map().goes_down = false;
        update_current_room_and_position(screen);
    }

    if (get_map().dimming > 0) {
        --get_map().dimming;
        get_map().Draw(screen);
        return;
    }

    get_map().goes_down = true;

    status = Status::GAME;
}

void Player::draw_empty_map(Image &screen)
{
    for (int y = coords.y; y < coords.y + player_tile_size; ++y) {
        for (int x = coords.x; x < coords.x + player_tile_size; ++x) {
            screen.PutPixel( x, screen.Height() - y - 1,
                             get_map().GetPixel(x, y) );
        }
    }
}


void Player::draw_over_game(Image &screen, const std::string &name)
{
    std::string base_dir = "./resources"; // TODO: гиперпараметр
    Image img(base_dir + "/" + name + ".png");
    Point start = get_center({get_map().Width(), get_map().Height()},
                             {img.Width(), img.Height()});

    if (name == "lose") {
        std::string tile_base_dir = "./resources/tileset";
        Image trap(tile_base_dir + "/" + "T_tile.png");

        int x1 = coords.x / tileSize, y1 = coords.y / tileSize;
        int x2 = (coords.x + tileSize - 1) / tileSize, y2 = (coords.y + tileSize - 1) / tileSize;
        int x0 = x1, y0 = y1;

        char symb1 = map.get_data()[y1][x1];
        char symb2 = map.get_data()[y2][x2];
        char symb3 = map.get_data()[y2][x1];
        char symb4 = map.get_data()[y1][x2];

        if (symb1 == 'T') {
            x0 = x1; y0 = y1;
        } else if (symb2 == 'T') {
            x0 = x2; y0 = y2;
        } else if (symb3 == 'T') {
            x0 = x1; y0 = y2;
        } else if (symb4 == 'T') {
            x0 = x2; y0 = y1;
        } else {
            x0 = -1;
        }

        if (x0 != -1) {
            for (int y = y0 * tileSize; y < (y0 + 1) * tileSize; y++) {
                for (int x = x0 * tileSize; x < (x0 + 1) * tileSize; x++) {
                    get_map().PutPixel(x, y,blend( get_map().GetPixel(x, y),
                            trap.GetPixel(x - x0 * tileSize, y - y0 * tileSize) ) );
                }
            }
        }
    }

    for (int y = 0; y < std::min( get_map().Height(), img.Height() ); y++) {
        for (int x = 0; x < std::min( get_map().Width(), img.Width() ); x++) {
            if (x + start.x < 0 || y + start.y < 0) {
                continue;
            }
            screen.PutPixel(x + start.x, screen.Height() - (y + start.y) - 1,
                            blend( get_map().GetPixel(x + start.x, y + start.y),
                            screen.GetPixel(x + start.x, screen.Height() - (y + start.y) - 1) ) );
            screen.PutPixel(x + start.x, screen.Height() - (y + start.y) - 1,
                    blend( screen.GetPixel(x + start.x, screen.Height() - (y + start.y) - 1),
                            img.GetPixel(x, y) ) );
        }
    }

    status = Status::OVER;
}


void Player::draw_main_game(Image &screen)
{
    if ( Moved() ) {
        for(int y = old_coords.y; y < old_coords.y + player_tile_size; ++y) {
            for(int x = old_coords.x; x < old_coords.x + player_tile_size; ++x) {
                screen.PutPixel(x, screen.Height() - y - 1,
                                get_map().GetPixel(x, y) );
            }
        }
        old_coords = coords;
        sprite.update_step();
    }

    for (int y = coords.y; y < coords.y + player_tile_size; ++y) {
        for (int x = coords.x; x < coords.x + player_tile_size; ++x) {
            screen.PutPixel( x, screen.Height() - y - 1,
                    blend(get_map().GetPixel(x, y),
                            sprite.tiles[sprite.get_step()]->GetPixel(x - coords.x, y - coords.y)
                             ) );
        }
    }
}


bool Player::can_move_next(Point p1, Point p2)
{
    char symb1 = map.get_data()[p1.y / tileSize][p1.x / tileSize];
    char symb2 = map.get_data()[p2.y / tileSize][p2.x / tileSize];

    return !( valid_for_step.find(symb1) == valid_for_step.end() ||
            valid_for_step.find(symb2) == valid_for_step.end() );
}


bool has_intersection(const std::pair<Point, Point> &p1, const std::pair<Point, Point> &p2)
{
    std::set<std::pair<int, int>> s1 = {
            { p1.first.x, p1.first.y },
            { p1.first.x, p1.second.y },
            { p1.second.x, p1.second.y },
            { p1.second.x, p1.first.y },
    };
    std::set<std::pair<int, int>> s2 = {
            { p2.first.x, p2.first.y },
            { p2.first.x, p2.second.y },
            { p2.second.x, p2.second.y },
            { p2.second.x, p2.first.y },
    };
    for (auto &p : s1) {
        if ( s2.find(p) != s2.end() ) {
            return true;
        }
    }
    return false;
}


void Enemy::prepare_for_game(bool is_blur, int repeat, Map &map)
{
    sprite.initialize(is_blur, repeat, "e");
    enemy_tile_size = sprite.tile_size;

    auto &symb_map = map.get_map_plan();
    start_coords = coords = { map.enemies[0].room_x * tileSize,
                map.enemies[0].room_y * tileSize };
}



void Enemy::Draw(Player &player, Image &screen)
{
    if ( player.get_map().current_room_type() != room_type ||
            player.get_current_status() == Status::MOVE) {
        coords = start_coords;
        return;
    }

    Point old_coords = coords;

    int dx = player.coords.x - coords.x;
    int dy = player.coords.y - coords.y;

    int bias_x = (dx == 0 ? 0 : (dx > 0 ? 1 : -1)) * move_speed;
    int bias_y = (dy == 0 ? 0 : (dy > 0 ? 1 : -1)) * move_speed;


    if ( abs(dx) > abs(dy) ) {
        if ( can_move_next(player, coords.x + bias_x, coords.y) ) {
            coords.x += bias_x;
        } else {
            coords.y += bias_y;
        }
    } else {
        if ( can_move_next(player, coords.x, coords.y + bias_y) ) {
            coords.y += bias_y;
        } else {
            coords.x += bias_x;
        }
    }

    if ( has_intersection(get_room_coords(coords.x, coords.y),
                     get_room_coords(player.coords.x, player.coords.y)) ) {
        player.set_current_status(Status::LOSS);
    }

    for(int y = old_coords.y; y < old_coords.y + sprite.tile_size; ++y) {
        for(int x = old_coords.x; x < old_coords.x + sprite.tile_size; ++x) {
            screen.PutPixel(x, screen.Height() - y - 1,
                            player.get_map().GetPixel(x, y) );
        }
    }

    for (int y = coords.y; y < coords.y + sprite.tile_size; ++y) {
        for (int x = coords.x; x < coords.x + sprite.tile_size; ++x) {
            screen.PutPixel( x, screen.Height() - y - 1,
                    blend(player.get_map().GetPixel(x, y),
                            sprite.tiles[sprite.get_step()]->GetPixel(x - coords.x, y - coords.y)
                             ) );
        }
    }

    sprite.update_step();
}
