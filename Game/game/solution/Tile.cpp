#include "Tile.h"


Tile::~Tile()
{
    while ( !tile_dict.empty() ) {
        auto it = tile_dict.begin();
        delete it->second;
        tile_dict.erase(it);
    }

    tile_dict.clear();
    name_to_symb.clear();
    names.clear();
}


bool Tile::load_tiles(const std::string &base_dir,
                      const std::vector<std::string> &names)
{
    for (const auto &s : names) {
        std::string pref;
        if ( !get_pref(s, pref) ) {
            std::cout << "load_tiles: getting filename preffix failed" << std::endl;
            return false;
        }
        auto it = name_to_symb.find(pref);
        if (it == name_to_symb.end()) {
            std::cout << "load_tiles: invalid prefix" << std::endl;
            return false;
        }
        char symb = it->second;

        if (symb == '.' || symb == 'T' || symb == '@' || symb == 'E') {
            tile_dict[symb] = new Image(BaseBlockSize, BaseBlockSize, 4);
            tile_dict[symb]->FillImage(EmptyColor); // backgroundColor
        } else {
            tile_dict[symb] = new Image(base_dir + "/" + s);
        }

        if(tile_dict[symb]->Size() == 0) {
            std::cout << "load_tiles: load Image for tile failed" << std::endl;
            return false;
        }
    }
    return true;
}


bool Tile::fix_tileset(const std::string &base_dir)
{
    return load_tiles(base_dir, names);
}


bool Tile::get_pref(const std::string &s, std::string &pref)
{
    size_t i = 0;
    while (s[i] != '_' && s[i] != '.') {
        pref += s[i];
        if (i == s.size()) {
            std::cout << "get_pref: symbol _ or . expected, not found" << std::endl;
            std::cout << "string: " << " " << s << std::endl;
            return false;
        }
        i++;
    }
    if (pref.size() == 0) {
        std::cout << "get_pref: empty prefix" << std::endl;
        return false;
    }
    return true;
}


void Sprite::initialize(bool is_blur, int repeat, const std::string &pref)
{
    std::unordered_map<std::string, char> n2s = {
            {pref + "0", '0'},
            {pref + "1", '1'},
            {pref + "2", '2'},
            {pref + "3", '3'},
            {pref + "4", '4'},
            {pref + "5", '5'}
    };
    std::vector<std::string> names = {
            pref + "0_tile.png",
            pref + "1_tile.png",
            pref + "2_tile.png",
            pref + "3_tile.png",
            pref + "4_tile.png",
            pref + "5_tile.png"
    };

    Tile tileset(n2s, names);

    std::string BaseResrcDir = "resources/tileset";
    if (  !tileset.load_tiles(BaseResrcDir, names) ) {
        std::cout << "Load tile failed" << std::endl;
    }
    for (auto &p : tileset.tile_dict) {
        if (p.second->Width() != p.second->Height()) {
            std::cout << "Tile width != tile height" << std::endl;
            break;
        }
    }

    step_count = tileset.tile_dict.size();
    this->repeat = repeat;
    tile_size = tileset.tile_dict.begin()->second->Width();
    tiles.resize(step_count);

    for (int i = 0; i < step_count; i++) {
        tiles[i] = tileset.tile_dict[char('0' + i)];
        tileset.tile_dict[char('0' + i)] = nullptr;
    }

    this->is_blur = is_blur;
    if (is_blur) {
        int r = 1;
        double sigma = Image::blur_sigma;
        auto filter = Image::getGaussian(r, sigma);
        for (int i = 0; i < step_count; i++) {
            tiles[i] = tiles[i]->conv2D(filter.get(), 2 * r + 1, 0);
        }
    }
}

Sprite::~Sprite()
{
    for (int i = 0; i < tiles.size(); i++) {
        delete tiles[i];
    }
    tiles.clear();
}