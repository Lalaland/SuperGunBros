import level

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
Enter the actual shape of the level. Notionally, it's a building in the center, a hotel on the side, and idk on the left\
We'll see how that works out.
'''
hotel_width = 800
hotel_elevator_width = 120
hotel_num_floors = 6
hotel_floor_thickness = 200
hotel_wall_thickness = 80
hotel_ceiling_height = 250
hotel_door_height = 140
hotel_balcony_thickness = 50
hotel_balcony_length = 250
hotel_balcony_rail = 30
hotel_roof_location = hotel_ceiling_height

for i in range(hotel_num_floors):

    floor_y = hotel_roof_location + i * (hotel_floor_thickness + hotel_ceiling_height)
    lobby = i == hotel_num_floors - 1
    roof = i == 0

    #Floor
    lev.add_obstacle(width - 20 - hotel_width,
                     width - 20 - hotel_elevator_width,
                     floor_y,
                     floor_y + hotel_floor_thickness)

    #Elevator Wall
    lev.add_obstacle(width - 20 - hotel_elevator_width - hotel_wall_thickness,
                     width - 20 - hotel_elevator_width,
                     floor_y + hotel_floor_thickness - 20,
                     height - 20 - 1.5 * hotel_door_height if lobby
                     else floor_y + hotel_floor_thickness + hotel_ceiling_height - hotel_door_height)

    #Outside wall
    lev.add_obstacle(width - 20 - hotel_width,
                     width - 20 - hotel_width + hotel_wall_thickness,
                     floor_y + hotel_floor_thickness - 20,
                     height - 20 - 2.0 * hotel_door_height if lobby
                     else floor_y + hotel_floor_thickness + hotel_ceiling_height - 1.5 * hotel_door_height)

    #Balcony
    if not roof:
        lev.add_obstacle(width - hotel_width - hotel_balcony_length - 20,
                         width - hotel_width,
                         floor_y,
                         floor_y + hotel_balcony_thickness)

        lev.add_obstacle(width - hotel_width - hotel_balcony_length - 20,
                         width - hotel_width - hotel_balcony_length - 20 + hotel_balcony_thickness/2,
                         floor_y - hotel_balcony_rail,
                         floor_y + hotel_balcony_thickness)


#Weapons
lev.add_box_spawn(width/2, height/2, ["rocket", "flame", "pierce", "bounce"])

#Spawns
for i in range(4):
    lev.add_player_spawn(width/2 + (1 if i%2 > 0 else -1) * (width/2 - 150), height - 150)
    #lev.add_player_spawn(width/2 + (1 if i > 0 else -1) * (width/2 - 150), 150)

print "Saving Level..."
lev.save("../"+title+".json", note = note)
print "done."