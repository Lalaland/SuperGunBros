import level
import pre_builts

#Note
note = "generated by bnb_gen.py"

#Settings
title = "Bunkers and Belltowers"
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
def build_box(x, y, w, h, m = False):
    lev.add_obstacle(x - w/2, x + w/2, y - h/2, y + h/2, m)

PLAT_TH = 50

def build_plat(x, y, w, mirror = False):
    build_box(x, y, w, PLAT_TH, m=mirror)

WALL_TH = 50

def build_wall(x, y, h, mirror = False):
    build_box(x, y, WALL_TH, h, m=mirror)


#Towers
QUAD_X = 500
QUAD_Y = 500

QUAD_W = 200

QUAD_Y_OFF = 200
QUAD_X_OFF = 400

def build_quad(x, y, w, x_off, y_off):
    for i in range(2):
        for j in range(2):
            curx = x + ((i+j)%2 * (x_off * (-1 if j != 0 else 1)))
            cury = y + ((i+j)%2 * (y_off * (-1 if i != 0 else 1)))
            build_plat(curx, cury, w)


build_quad(QUAD_X, QUAD_Y, QUAD_W, QUAD_X_OFF, QUAD_Y_OFF)
#Center
CENTER_W = 400
CENTER_H = height - 1000
build_plat(width/2, CENTER_H, CENTER_W, mirror=False)

'''
Spawns
'''

#For now, all clustered on same plats
lev.add_player_spawn(QUAD_X, QUAD_Y)

for i in range(2):
    lev.add_player_spawn(width/2 + (CENTER_W/4 * (1 if i%2 > 0 else -1)), CENTER_H - 60, mirror=False)

'''
Weapons
'''
lev.add_box_spawn(width/2, CENTER_H - 60, ["rocket"], mirror=False)

print "Saving complex..."
lev.save("../bnb.json", note = note)