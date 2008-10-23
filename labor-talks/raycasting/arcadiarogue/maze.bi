#define MAZESIZE 31


enum
    TILE_WALL   = 0
    TILE_FLOOR
    TILE_DOOR
    TILE_DOORCLOSED = TILE_DOOR
    TILE_DOOROPEN
    TILE_STAIRSUP
    TILE_STAIRSDOWN
    TILE_GRASS
    TILE_NOTHING 
end enum

dim shared as integer TileColorTab(0 to 127) = _
        {9, _   'Wall
        8, _    'Floor
        6, _    'Door Closed
        6, _    'Door Open
        7, _    'Stairs Up
        7, _    'Stairs Down
        2, _
        1}      'Void
dim shared as integer TileCharTab(0 to 127) = _
        {128,129,130,131,132,133,134,128} 

'TileColorTab(127) = 1
type itemtype
    id as integer
    curchar as integer 
    x as integer
    y as integer
end type

type mazetype
    tile(0 to MAZESIZE,0 to MAZESIZE,0 to MAZESIZE) as integer
    declare function gettile(x as integer,y as integer, z as integer) as integer
    seen(0 to MAZESIZE,0 to MAZESIZE,0 to MAZESIZE) as integer
    declare function getseen(x as integer,y as integer, z as integer) as integer
    moblookup(0 to MAZESIZE,0 to MAZESIZE,0 to MAZESIZE) as integer
    
    
    walltile(0 to MAZESIZE) as integer
    floortile(0 to MAZESIZE) as integer
    walltilecolor(0 to MAZESIZE) as integer
    floortilecolor(0 to MAZESIZE) as integer
    bordertile(0 to MAZESIZE) as integer
    bordertilecolor(0 to MAZESIZE) as integer
    
    mazename(0 to MAZESIZE) as string
    
    declare sub DoLOStoSeen(x as integer,y as integer,z as integer,r as double)
    
    declare function CheckCollision(dx as integer,dy as integer,dz as integer) as integer
    
    declare sub OpenDoor(x as integer,y as integer,z as integer)
    
    declare sub create()
    declare sub createboxmaze(floor as integer)
    declare sub createcavern(floor as integer)
    declare sub createcavern_carveradius(posx as double, posy as double, radius as double, floor as integer)
    startx as integer
    starty as integer
    declare sub createplayerstart()
    declare sub createstairs(floor as integer)
end type

enum
    COLL_UNTESTED = -1
    COLL_NONE
    COLL_STAIRSUP
    COLL_STAIRSDOWN
    COLL_WALL
    COLL_CLOSEDDOOR
    COLL_MONSTER
end enum

function mazetype.CheckCollision(dx as integer,dy as integer,dz as integer) as integer
    if dx < 0 or dx > MAZESIZE or dy < 0 or dy > MAZESIZE or dz < 0 or dz > MAZESIZE then return COLL_WALL
    select case gettile(dx,dy,dz)
    case TILE_WALL
        return COLL_WALL
    case TILE_FLOOR
        return COLL_NONE
    case TILE_DOORCLOSED
        return COLL_CLOSEDDOOR
    case TILE_DOOROPEN
        return COLL_NONE
    case TILE_STAIRSUP
        return COLL_STAIRSUP
    case TILE_STAIRSDOWN
        return COLL_STAIRSDOWN
    case TILE_GRASS
        return COLL_NONE
    case TILE_NOTHING
        return COLL_WALL
    end select
end function

sub mazetype.DoLOStoSeen(x as integer,y as integer,z as integer,r as double)
    dim as integer angle
    dim as double px,py,sx,sy,currad
    for px = 0 to MAZESIZE
        for py = 0 to MAZESIZE
            if seen(px,py,z) = 1 then seen(px,py,z) = 2
        next py
    next px
    for angle = 0 to 360 step 1
        sx = sin(angle*PI/180)*0.2
        sy = cos(angle*PI/180)*0.2
        px = x+0.5
        py = y+0.5
        for currad = 0 to r step 0.2
            if int(px) < 0 or int(px) > MAZESIZE or int(py) < 0 or int(py) > MAZESIZE then exit for
            seen(int(px),int(py),z) = 1
            if tile(int(px),int(py),z) = TILE_WALL or tile(int(px),int(py),z) = TILE_DOORCLOSED then exit for
            px += sx
            py += sy
        next currad
    next angle
    
end sub

sub mazetype.OpenDoor(x as integer,y as integer,z as integer)
    tile(x,y,z) = TILE_DOOROPEN
end sub
#include "maze_boxmaze.bi"
#include "maze_cavernmaze.bi"

function mazetype.gettile(x as integer,y as integer, z as integer) as integer
    if x < 0 or x > MAZESIZE or y < 0 or y > MAZESIZE or z < 0 or z > MAZESIZE then return TILE_NOTHING
    return tile(x,y,z)
end function

function mazetype.getseen(x as integer,y as integer, z as integer) as integer
    if x < 0 or x > MAZESIZE or y < 0 or y > MAZESIZE or z < 0 or z > MAZESIZE then return 1
    return seen(x,y,z)
end function

sub mazetype.createstairs(floor as integer)
    dim as integer x,y
    dim as integer dx,dy
    do
        x = int(rnd*MAZESIZE)
        y = int(rnd*MAZESIZE)
        if tile(x,y,floor) = TILE_FLOOR then exit do
    loop
    tile(x,y,floor) = TILE_STAIRSUP
    if floor = 0 then
        startx = x
        starty = y
    end if

    do
        do
            dx = int(rnd*MAZESIZE)
            dy = int(rnd*MAZESIZE)
            if tile(dx,dy,floor) = TILE_FLOOR then exit do
        loop
        if abs(x-dx) > 5 or abs(y-dy) > 5 then exit do
    loop
    tile(dx,dy,floor) = TILE_STAIRSDOWN
    
end sub

sub mazetype.create()
    dim as integer x,y,z
    gfx.print 0,0,"Creating...",15,1
    gfx.show
    createboxmaze(0)
    createstairs(0)
    mazename(0) = "Ruins"

    createcavern(1)
    createstairs(1)
    mazename(1) = "Caverns"
    
    if DEBUG = 1 then
        locate 1,1
        for y = 0 to MAZESIZE
            for x = 0 to MAZESIZE
                select case tile(x,y,0)
                case 0
                    print "#";
                case 1
                    print ".";
                case 2
                    print "/";
                case 3
                    print "\";
                case 4
                    color RGB(255,0,0)
                    print "<";
                    color RGB(255,255,255)
                case 5
                    color RGB(255,0,0)
                    print ">";
                    color RGB(255,255,255)
                case else
                    print "?";
                end select
            next x
            print
        next y    

        sleep
    end if ' DEBUG

    cls

    
end sub

