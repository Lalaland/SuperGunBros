import level
import pre_builts

#Note
note = "generated by complex_gen.py"

#Settings
title = "Complex"
width = 5760
height = 3249
mirrored = False

#Init
lev = level.Level(title, width, height, mirrored)

'''
Outer Bounds
'''
#Floor
lev.add_obstacle(-20, width + 20, height - 20, height + 20)
#Ceiling
lev.add_obstacle(-20, width + 20, -20, 20)
#Walls
lev.add_obstacle(-20, 20, -20, height + 20, mirror = True)

'''
Hotel
'''
pre_builts.build_hotel(lev)

'''
Central Compound
'''

complex_width = width/2
cc_center_x = (complex_width/2) - 20
cc_outer_wall = 40
lev.add_obstacle(cc_center_x - complex_width/2 - cc_outer_wall,
                 cc_center_x + complex_width/2 + cc_outer_wall,
                 height/3 - cc_outer_wall,
                 height/3 + cc_outer_wall)


'''
Whatever I put on the lef
'''


#Spawns
for i in range(4):
    lev.add_player_spawn(width/2 + (1 if i%2 > 0 else -1) * (width/2 - 150), height - 150)
    #lev.add_player_spawn(width/2 + (1 if i > 0 else -1) * (width/2 - 150), 150)

print "Saving complex..."
lev.save("../complex.json", note = note)