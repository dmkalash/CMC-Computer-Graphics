map {
  _filename = "Levels/intro.png"
}

texture {
  _name = "walls_tex"
  _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_59.png"
}

texture {
  _name = "footman_tex"
  _filename = "tileset/animations/footman.png"
}

texture {
  _name = "death_knight_tex"
  _filename = "tileset/animations/death_knight.png"
}

texture {
  _name = "peasant_tex"
  _filename = "tileset/animations/peasant.png"
}

texture {
  _name = "knight_tex"
  _filename = "tileset/animations/knight.png"
}

texture {
  _name = "mage_tex"
  _filename = "tileset/animations/mage.png"
}

texture {
  _name = "point_tex"
  _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_432.png"
}

tile {
  _name = "walls_tile"
  _texture = "walls_tex"
}
tile {
  _name = "point"
  _texture = "point_tex"
} 
entity {
  _type = "Walls"
  _tile = "walls_tile"
  _patterned = true
  _colorBinding = #FFFFFF
}

entity {
  _type = "Player"
  _animation = "4 death_knight_tex"
  CompPos = 2/2
  CompDir = 0/1
  CompMana = 100/0.5
  CompHealth = 300/0.5
}
entity {
  _type = "Enemy"
  _animation = "4 footman_tex"
  _colorBinding = #0000FF
  CompHealth = 200/0.5
  CompArmor = 0.5/0
  CompDamage = 10/10
  CompVisionRadius = 5
}

entity {
  _type = "Enemy"
  _animation = "5 peasant_tex"
  _colorBinding = #00FF00
  CompHealth = 50/0.5
  CompArmor = 0/0
  CompDamage = 2/2
  CompVisionRadius = 2
}   
       
entity {
  _type = "KnightBoss"
  _animation = "4 knight_tex"
  CompPos = 3/9
  CompDir = 0/1
  CompHealth = 600/1
  CompMana = 10/0
  CompArmor = 0/0
  CompDamage = 50/0
  CompVisionRadius = 5
}  
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 2/2
  CompTrigger = 0
  CompMessedge = "Training.+Move+the+hero+on+W,A,S,D."
}

entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 2/3
  CompTrigger = 0
  CompMessedge = "To+attack+press+arrows+up,+down,+right,+left.+You+have+9+items+slots:+weapons,+equipments+or+potions.+Press+the+key+1+to+put+on+the+Death+Wand,+a+description+of+weapon+and+its+ability+will+appear+at+the+bottom." 
} 
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 2/4
  CompTrigger = 0
  CompMessedge = "Press+Q+and+arrow+Right+to+cast+Death+Coil.+Death+Coil+flies+in+initial+direction.+If+it+hit+enemy,+enemy+get+damage+and+Death+Knight+restore+some+health.+Remember+it+takes+mana."
}
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 7/4
  CompTrigger = 0
  CompMessedge = "Ahead+is+a+detachment+of+the+living,+destroy+it!"
}
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 16/7
  CompTrigger = 0
  CompMessedge = "This+village+is+full+of+people,+KILL+THEM+ALL!+Their+guard+will+not+help+them."
}
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 8/13
  CompTrigger = 0
  CompMessedge = "I+find+you,+Tommy!"
}
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 3/16
  CompTrigger = 0
  CompMessedge = "The+stupid+knight+thinks+that+the+armor+will+save+him+from+death.+Ha-ha-ha!!!"
}
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 3/8
  CompTrigger = 1
  CompMessedge = "Level+passed!*Soon+the+whole+world+will+submit+to+the+power+of+the+Death-King!!!"
}


  
