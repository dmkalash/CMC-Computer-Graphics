map {
  _filename = "Levels/orda_floor.png"
}

texture {
  _name = "grass1t"
  _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_1968.png"
}

texture {
  _name = "grass2t"
  _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_1969.png"
}
texture {
  _name = "plate1t"
  _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_1381.png"
}
texture {
  _name = "plate2t"
  _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_1382.png"
}
texture {
  _name = "plate3t"
  _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_1383.png"
}

tile {
  _name = "grass1"
  _texture = "grass1t"
}

tile {
  _name = "grass2"
  _texture = "grass2t"
}
tile {
  _name = "plate1"
  _texture = "plate1t"
}
tile {
  _name = "plate2"
  _texture = "plate2t"
}
tile {
  _name = "plate2"
  _texture = "plate2t"
}
tile {
  _name = "plate3"
  _texture = "plate3t"
}
entity {
  _type = "Floor"
  _tile = "grass1"
  _patterned = true
  _colorBinding = #00FF00
}

entity {
  _type = "Floor"
  _tile = "grass2"
  _patterned = true
  _colorBinding = #008000
}

entity {
  _type = "Floor"
  _tile = "plate1"
  _patterned = true
  _colorBinding = #808080
}
entity {
  _type = "Floor"
  _tile = "plate2"
  _patterned = true
  _colorBinding = #8F8F8F
}
entity {
  _type = "Floor"
  _tile = "plate3"
  _patterned = true
  _colorBinding = #000000
}


