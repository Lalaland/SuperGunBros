import level
from pre_builts import build_box

#Note
note = "generated by bnb_gen.py"

#Settings
title = "Refuge"
width = 3840
height = 2160

mirrored = True

#Init
lev = level.Level(title, width, height, mirrored)

'''
Killing floor
'''
lev.add_kill_box(-10*width, 11*width, height + 100, 2*height, mirror=False)

'''
Plats / Walls
'''
PLAT_TH = 50

def bb(x, y, w, h, mirror=None, pierceable=None):
    build_box(lev, x, y, w, h, mirror=mirror, pierceable=pierceable)

def build_plat(x, y, w, mirror=None, pierceable=None):
    bb(x, y, w, PLAT_TH, mirror=mirror, pierceable=pierceable)

WALL_TH = 50

def build_wall(x, y, h, mirror=None, pierceable=None):
    bb(x, y, WALL_TH, h, mirror=mirror, pierceable=pierceable)


#Towers
TOWER_X = 500
TOWER_H = 600

TOWER_FLOOR_W = 220

TOWER_Y_SEP = 150
TOWER_X_SEP = 170

TOWER_FLOORS = 8

def build_tower(x, y, w, x_sep, y_sep, num_steps, mirror=None):
    for i in range(num_steps):
        build_plat(x + (w/2 + x_sep/2)*(1 if i%2 > 0 else -1), y + y_sep * i, w, mirror=mirror)



build_tower(TOWER_X, TOWER_H, TOWER_FLOOR_W, TOWER_X_SEP, TOWER_Y_SEP, TOWER_FLOORS)


#Center
BUNKER_W = 2020
BUNKER_H = height - 800
build_plat(width/2, BUNKER_H, BUNKER_W, mirror=False, pierceable=False)

#Underground
NUM_UNDG = 6
UNDG_HEIGHT = BUNKER_H + 400
UNDG_PLAT_W = 200
UNDG_LENGTH = BUNKER_W
UNDG_GAP = 450
for i in range(NUM_UNDG/2):
    build_plat(width/2 - UNDG_LENGTH/2 + i * UNDG_GAP + UNDG_PLAT_W/2, UNDG_HEIGHT, UNDG_PLAT_W)


#WALLS?
WALL_X = TOWER_X + TOWER_X_SEP + TOWER_FLOOR_W + 50
INNER_WALL_H = TOWER_H + (TOWER_FLOORS/2 - 3) * TOWER_Y_SEP
OUTER_WALL_H = TOWER_H + (TOWER_FLOORS + 2) * TOWER_Y_SEP
INNER_WALL_Y = TOWER_H + INNER_WALL_H/2 - 300
OUTER_WALL_Y = TOWER_H + OUTER_WALL_H/2 - 300

build_wall(WALL_X, INNER_WALL_Y, INNER_WALL_H)
build_wall(2*TOWER_X - WALL_X, OUTER_WALL_Y, OUTER_WALL_H)
build_plat(TOWER_X, INNER_WALL_Y - INNER_WALL_H/2 + PLAT_TH/2, (WALL_X - TOWER_X) * 2)


'''
Spawns
'''

#Bunker Spawns
lev.add_player_spawn(width/2 - UNDG_LENGTH/2 + UNDG_PLAT_W/2, UNDG_HEIGHT - 80)

#Roof of bunker
lev.add_player_spawn(width/2 + (BUNKER_W * 2/7), BUNKER_H - 80)

'''
Weapons
'''
lev.add_box_spawn(TOWER_X - TOWER_X_SEP/2 - TOWER_FLOOR_W/2, TOWER_H - 60, ["pierce"])
lev.add_box_spawn(width/2, BUNKER_H - 60, ["rocket"], mirror=False)
lev.add_box_spawn(width/2, UNDG_HEIGHT - 60, ["bounce"], mirror=False)

print "Saving complex..."
lev.save("../bnb.json", note = note)