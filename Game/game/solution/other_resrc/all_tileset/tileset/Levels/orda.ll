map {
  _filename = "Levels/orda.png"
}

texture {
  _name = "walls_tex"
  _filename = "tileset/Sliced/world_24x24/oryx_16bit_fantasy_world_344.png"
}


texture {
  _name = "orc_tex"
  _filename = "tileset/animations/grunt.png"
}

texture {
  _name = "troll_tex"
  _filename = "tileset/animations/troll.png"
}

texture {
  _name = "deamon_tex"
  _filename = "tileset/animations/daemon.png"
}

texture {
  _name = "death_knight_tex"
  _filename = "tileset/animations/death_knight.png"
}

texture {
  _name = "ogre_tex"
  _filename = "tileset/animations/ogre.png"
}

texture {
  _name = "peon_tex"
  _filename = "tileset/animations/peon.png"
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
  CompPos = 7/7
  CompDir = 0/1
  CompMana = 100/0.5
  CompHealth = 300/0.5
}
entity {
  _type = "OgreBoss"
  _animation = "4 ogre_tex"
  CompPos = 51/28
  CompDir = 1/0
  CompMana = 100/0.5
  CompHealth = 300/0.5
  CompArmor = 0/0.7
  CompDamage = 2/10
  CompVisionRadius = 5
}
entity {
  _type = "OgreBoss"
  _animation = "4 ogre_tex"
  CompPos = 7/39
  CompDir = 1/0
  CompMana = 100/1
  CompHealth = 400/0.5
  CompArmor = 0/1
  CompDamage = 2/20
  CompVisionRadius = 5
}
entity {
  _type = "DeamonBoss"
  _animation = "5 deamon_tex"
  CompPos = 52/7
  CompDir = 1/0
  CompMana = 100/.6
  CompHealth = 600/2
  CompArmor = 1/0
  CompDamage = 30/30
  CompVisionRadius = 5
}
entity {
  _type = "Enemy"
  _animation = "4 orc_tex"
  _colorBinding = #FF0000
  CompHealth = 200/0.5
  CompArmor = 0.3/0
  CompDamage = 20/0
  CompVisionRadius = 5
}   
       
entity {
  _type = "Enemy"
  _animation = "4 troll_tex"
  _colorBinding = #00FFFF 
  CompHealth = 100/0.9
  CompArmor = 0/0
  CompDamage = 30/0
  CompVisionRadius = 5
}  
 
entity {
  _type = "Enemy"
  _animation = "5 peon_tex"
  _colorBinding = #FFFF00
  CompHealth = 50/0.1
  CompArmor = 0/0
  CompDamage = 5/0
  CompVisionRadius = 2
} 
entity {
  _type = "Enemy"
  _animation = "4 ogre_tex"
  _colorBinding = #0000FF 
  CompHealth = 200/0.3
  CompArmor = 0/0.5
  CompDamage = 2/20
  CompVisionRadius = 5
} 

entity {
  _type = "Enemy"
  _animation = "5 deamon_tex"
  _colorBinding = #FF00FF 
  CompHealth = 200/1
  CompArmor = 1/0.3
  CompDamage = 20/20
  CompVisionRadius = 7
} 
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 7/7
  CompTrigger = 0
  CompMessedge = "Now,+the+greenskins+will+join+the+Army+of+the+Death."
}
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 7/44
  CompTrigger = 1
  CompMessedge = "You+killed+this+Ogre-Mage.+It’s+time+to+show+the+greenskin+horde+that+no+magic+can+save+them+from+Death!"
}
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 36/33
  CompTrigger = 0
  CompMessedge = "Fool+stupid+orcs.+Come+back+in+to+kill+Ogre-King."
}
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 52/14
  CompTrigger = 2
  CompMessedge = "Crazy+orcs+worship+the+demon.*Daemon!+Daemon!+Daemon!*Destroy+Betrezen's+servant+for+the+glory+of+the+Dead-Lord!!!"
}
entity {
  _type = "Trigger"
  _tile = "point"
  CompPos = 52/7
  CompTrigger = 3
  CompMessedge = "Orcs+and+Daemon+are+defeated!+Death-Knight+is+dood+boy."
}


  
