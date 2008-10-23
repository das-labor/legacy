
sub mazetype.createcavern(floor as integer)
    dim as double stepx,stepy,posx,posy
    dim as double radius,dx,dy,tp
    dim as integer counter,ntiles,chockecounter
    dim as integer x,y
    'Say what tiles this maze uses
    walltile(floor) = 134
    walltilecolor(floor) = 7
    floortile(floor) = 135
    floortilecolor(floor) = 8
    bordertile(floor) = 134
    bordertilecolor(floor) = 1
    
    for dx = 0 to MAZESIZE
        for dy = 0 to MAZESIZE
            tile(dx,dy,floor) = TILE_WALL
        next dy
    next dx

    for x = 5 to MAZESIZE step 10
        for y = 5 to MAZESIZE step 10
            posx = x+0.5
            posy = y+0.5
            radius = (int(rnd*5)+5)/2
            createcavern_carveradius(posx, posy, radius, floor)
        next y
    next x


    for dx = -1 to 1
        for x = 5 to MAZESIZE step 20
            for y = 5 to MAZESIZE-5
                tile(x+dx,y,floor) = TILE_FLOOR
            next y
        next x
        for y = 5 to MAZESIZE step 20
            for x = 5 to MAZESIZE-5
                tile(x,y+dx,floor) = TILE_FLOOR
            next x
        next y
    next dx

    dy = int(rnd*100)+1
    if dy < 33 then dx = 1
    if dy >= 33 and dy < 66 then dx = 2
    if dy >= 66 then dx = 3

    if dx = 1 or dx = 3 then
        for y = 14 to 16
            for x = 5 to MAZESIZE-5
                tile(x,y,floor) = TILE_FLOOR
            next x
        next y
    end if
    if dx = 2 or dx = 3 then
        for x = 14 to 16
            for y = 5 to MAZESIZE-5
                tile(x,y,floor) = TILE_FLOOR
            next y
        next x
    end if

end sub

sub mazetype.createcavern_carveradius(posx as double, posy as double, radius as double, floor as integer)
    dim as double angle, stepx,stepy,dx,dy,tp
    for angle = 0 to 360
        stepx = sin(angle*PI/180)
        stepy = cos(angle*PI/180)
        dx = posx
        dy = posy
        for tp = 0 to radius
            tile(int(dx),int(dy),floor) = TILE_FLOOR
            dy += stepx
            dx += stepy
            if dx < 1 or dx > MAZESIZE or dy < 1 or dy > MAZESIZE then tp=radius
        next tp
    next angle
end sub
