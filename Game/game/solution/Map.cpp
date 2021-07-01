#include "Map.h"
#include <memory>
#include <cmath>


void Map::blur_map(int r)
{
    auto filter = Image::getGaussian(r, Image::blur_sigma);
    for (auto &p : pixel_rooms_map) {
        Image *t = p.second->conv2D(filter.get(), 2 * r + 1, tileSize);
        p.second = t;
    }
}


bool Map::load_map(const std::string &base_dir, const std::string &map_path)
{
    if (map_path.empty()) {
        std::cout << "empty path" << std::endl;
        return false;
    }

    if ( !set_map_plan(base_dir, map_path) ) {
        return false;
    }

    if ( !get_rooms_plan(base_dir) ) {
        return false;
    }

    map_height = RoomSize * tileSize;
    map_width = RoomSize * tileSize;

    map_print();

    return true;
}


bool Map::set_map_plan(const std::string &base_dir,
                       const std::string &map_path)
{
    std::ifstream fin(base_dir + "/" + map_path);
    std::string row;
    fin >> row;

    if (row.empty()) {
        std::cout << "failed invariant: " << "map file empty" << std::endl;
        return false;
    }
    map_plan.push_back(row);
    size_t row_len = row.size();

    while (fin.good()) {
        fin >> row;
        if ( fin.fail() ) {
            break;
        }
        if (row.size() != row_len) {
            std::cout << "failed invariant: " << "different lens of map rows" << std::endl;
            return false;
        }
        map_plan.push_back(row);
    }
    return true;
}


bool Map::get_rooms_plan(const std::string &base_dir)
{
    std::set<char> room_types;
    for (auto &r : map_plan) {
        for (char c : r) {
            room_types.insert(c);
        }
    }

    std::string row;
    for (char c : room_types) {
        std::string pref = get_prefix(c);

        std::string filename = pref + "_room.txt";
        std::ifstream room_fin(base_dir + "/" + filename);

        std::vector<std::string> room_map;
        room_fin >> row;
        size_t room_row_size = row.size();
        if (RoomSize == 0) {
            RoomSize = room_row_size;
        }
        if (room_row_size == 0) {
            std::cout << "failed invariant: " << "room file empty" << std::endl;
            return false;
        }
        room_map.push_back(row);

        while (room_fin.good()) {
            room_fin >> row;
            if ( room_fin.fail() ) {
                break;
            }
            if (row.size() != room_row_size) { // invariant
                std::cout << "failed invariant: " << "different room rows lens" << std::endl;
                return false;
            }
            room_map.push_back(row);
        }

        if (room_map.size() != room_row_size) { // invariant
            std::cout << "failed invariant: " << "rooms must be squares" << std::endl;
            return false;
        }
        if (room_row_size != RoomSize) { // invariant
            std::cout << "failed invariant: " << "rooms size must be equal RoomSize" << std::endl;
            return false;
        }

        rooms_plan[c] = std::move(room_map);
    }
    return true;
}


std::string Map::get_prefix(char c)
{
    if (c == '.') {
        return "point";
    } else if (c == '@') {
        return "at";
    } else if (c == '#') {
        return "sharp";
    } else if (c == ' ') {
        return "blank";
    } else {
        return std::string() + c;
    }
}


void Map::map_print()
{
    std::cout << "MAP PLAN size: " << map_plan.size() << " " << map_plan[0].size() << std::endl;
    for (int i = 0; i < map_plan.size(); i++) {
        for (int j = 0; j < map_plan[0].size(); j++) {
            std::cout << map_plan[i][j];
        }
        std::cout << std::endl;
    }
}


bool Map::set_pixel_map(Tile &tiles)
{
    for (std::string &s : map_plan) {
        for (char room_type : s) {
            if (pixel_rooms_map.find(room_type) != pixel_rooms_map.end()) {
                continue;
            }
            auto &room = rooms_plan[room_type];
            pixel_rooms_map[room_type] = new Image(map_width, map_height, 4);

            for (size_t i = 0; i < RoomSize; i++) {
                for (size_t j = 0; j < RoomSize; j++) {
                    char symb = room[i][j];

                    auto it = tiles.tile_dict.find(symb);
                    if (it == tiles.tile_dict.end()) {
                        std::cout << "set_pixel_map: symbol not found - " << symb << std::endl;
                        return false;
                    }
                    Image *image = it->second;

                    for (size_t tile_i = 0; tile_i < tileSize; tile_i++) {
                        for (size_t tile_j = 0; tile_j < tileSize; tile_j++) {
                            pixel_rooms_map[room_type]->PutPixel(j * tileSize + tile_j,
                                                                    i * tileSize + tile_i,
                                                                    image->GetPixel(tile_j, tile_i) );
                        }
                    }
                }
            }
        }
    }

    return true;
}


void Map::Draw(Image &screen)
{
    for(int y = 0; y < map_height; ++y) {
        for(int x = 0; x < map_width; ++x) {
            screen.PutPixel(x, screen.Height() - y - 1,GetPixel(x, y) );
        }
    }
}


bool Map::initialize(Image &screen)
{
    load_map("maps", "level1.txt");

    Tile tileset;
    const char *BaseResrcDir = "resources/tileset"; // TODO: вынести в гиперпараметр
    tileset.fix_tileset(BaseResrcDir);

    set_pixel_map(tileset);

    if (map_height > screen.Height() ||
        map_width > screen.Width()) {
        std::cout << "MAP INITIALIZE: Map size must be not upper than screen size" << std::endl;
        std::cout << map_height << " vs " << screen.Height() << "\n";
        std::cout << map_width << " vs " << screen.Width() << "\n";
        return false;
    }

    return true;
}


void Map::scan()
{
    for (auto &char_vect : rooms_plan) {
        std::vector<std::string> &symb_map = char_vect.second;
        for (int y = 0; y < symb_map.size(); y++) {
            for (int x = 0; x < symb_map[0].size(); x++) {
                char symb = symb_map[y][x];
                if (symb == 'I') {
                    illum.push_back( {char_vect.first, x, y} );
                } else if (symb == '@') {
                    player_start = {char_vect.first, x, y};
                } else if (symb == 'E') {
                    enemies.push_back( {char_vect.first, x, y} );
                }
            }
        }
    }
}


void Map::set_illumination()
{
    int r = 256;
    for (MapPoint &p : illum) {
        pixel_rooms_map[p.room_type]->apply_matrix(p.room_x * tileSize + tileSize / 2,
                                                p.room_y * tileSize + tileSize / 2,
                                                    r);
    }
}


bool Map::add_background(Image &bg_image)
{
    int y_up = std::min(bg_image.Height(), Height());
    int x_up = std::min(bg_image.Width(), Width());

    for (auto &p : pixel_rooms_map) {
        Image *img = p.second;
        for(int y = 0; y < y_up; ++y) {
            for(int x = 0; x < x_up; ++x) {
                img->PutPixel(x, y_up - y - 1,
                         blend( bg_image.GetPixel(x, y_up - y - 1),
                                GetPixel(p.first, x, y_up - y - 1) ) );
            }
        }
    }

    return true;
}


Pixel Map::GetPixel(int x, int y)
{
    if (dimming != 0) {
        return pixel_rooms_map[ map_plan[map_x][map_y] ]->GetPixel(x, y, 1 - dimming * dimming_coef); //
    }
    return pixel_rooms_map[ map_plan[map_x][map_y] ]->GetPixel(x, y);
}

Pixel Map::GetPixel(char room_type, int x, int y)
{
    return pixel_rooms_map[ room_type ]->GetPixel(x, y); // TODO: хранить также текущий room_type
}


void Map::PutPixel(int x, int y, const Pixel &pix)
{
    pixel_rooms_map[ map_plan[map_x][map_y] ]->PutPixel(x, y, pix);
}



Map::~Map()
{
    illum.clear();
    enemies.clear();
    rooms_plan.clear();
    map_plan.clear();
    for (auto &p : pixel_rooms_map) {
        delete p.second;
    }
    pixel_rooms_map.clear();
}
