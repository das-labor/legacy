declare sub NewGame()
declare sub MainGame()
declare sub ShowMaze(x as integer, y as integer,z as integer)
declare sub ShowUI()
#include "maze.bi"
dim shared maze as mazetype

enum
    TYP_FIGHTER = 0
    TYP_PRIEST
    TYP_WIZARD
    TYP_ENDOFTYPES
end enum

dim shared PlayerCharTypeStrings(0 to TYP_ENDOFTYPES) as string
PlayerCharTypeStrings(TYP_FIGHTER) = "Fighter"
PlayerCharTypeStrings(TYP_PRIEST) = "Priest"
PlayerCharTypeStrings(TYP_WIZARD) = "Wizard"

type playertype
    name as string
    x as integer
    y as integer
    z as integer
    hp as double
    sp as double
    curatt as integer
    curdef as integer
    curchar as integer
    lightradius as double
    declare sub move(dx as integer, dy as integer, dz as integer)
    declare constructor
end type

sub playertype.move(dx as integer, dy as integer, dz as integer) 
    x += dx
    y += dy
    z += dz
end sub

constructor playertype
    lightradius = 3
    curchar = 0
    name = "Juremo"
    hp = 20
    sp = 5
end constructor

dim shared player as playertype

sub NewGame()
    maze.create
    player.x = maze.startx
    player.y = maze.starty
    player.z = 0
    MainGame()
end sub


sub MainGame()
    dim as integer redraw,lastcollision,deltax,deltay,playermoves
    dim as string lastkey
    redraw = 1
    do
        if redraw = 1 then
            maze.DoLOStoSeen(player.x,player.y,player.z,player.lightradius)
            ShowMaze(player.x,player.y,player.z)
            ShowUI()
            redraw = 0
        endif
        
        do
        sleep 1
        lastkey = inkey
        loop until lastkey <> ""
        'TODO: Keymapping support
        playermoves = 0
        select case lastkey
        ''''''''''''''''''''''''''' Movement
        case "8" 'N
            playermoves = 1
            deltax = 0 
            deltay = -1
        case "9" 'NE
            playermoves = 1
            deltax = 1 
            deltay = -1
        case "6" 'E
            playermoves = 1
            deltax = 1
            deltay = 0
        case "3" 'SE
            playermoves = 1
            deltax = 1 
            deltay = 1
        case "2" 'S
            playermoves = 1
            deltax = 0 
            deltay = 1
        case "1" 'SW
            playermoves = 1
            deltax = -1 
            deltay = 1
        case "4" 'W
            playermoves = 1
            deltax = -1 
            deltay = 0
        case "7" 'NW
            playermoves = 1
            deltax = -1 
            deltay = -1
        case "q"
            'TODO: Ask for real quit...
            exit do
        end select

        if playermoves = 1 then
            playermoves = 0
            redraw = 1
            lastcollision = maze.CheckCollision(player.x+deltax,player.y+deltay,player.z)
            select case lastcollision
            case COLL_NONE
                player.move (deltax,deltay,0)
                'gfx.AddToMessages(" ")
            case COLL_STAIRSUP
                player.move (deltax,deltay,0)
                gfx.AddToMessages("Stairs upwards.")
            case COLL_STAIRSDOWN
                player.move (deltax,deltay,0)
                gfx.AddToMessages("Stairs downwards.")
            case COLL_CLOSEDDOOR
                maze.OpenDoor(player.x+deltax,player.y+deltay,player.z)
                gfx.AddToMessages("Opening door.")
            case else
                gfx.AddToMessages("Blocked!")
            end select
        end if

        
    loop
end sub

sub ShowUI()
    with player
        'Name
        gfx.print 11*8,0,left(.name,8),15,1
        'ClassType
        gfx.print 11*8,8,PlayerCharTypeStrings(.curchar),15,1
        'Health
        gfx.print 11*8,16,chr(3),9,1
        gfx.print 12*8,16,space(3-len(trim(str(int(.hp)))))+trim(str(.hp)),15,1
        'and Mana
        gfx.print 15*8,16,chr(5),11,1
        gfx.print 16*8,16,space(3-len(trim(str(int(.sp)))))+trim(str(.sp)),15,1
        'Current Attack
        gfx.print 11*8,24,chr(6),1,1
        gfx.print 12*8,24,space(3-len(trim(str(.curatt))))+trim(str(.curatt)),15,1
        'and current Defence
        gfx.print 15*8,24,chr(4),7,1
        gfx.print 16*8,24,space(3-len(trim(str(.curdef))))+trim(str(.curdef)),15,1

        
    end with
    gfx.show
end sub

sub ShowMaze(x as integer, y as integer,z as integer)
    dim as integer dx,dy,dt,dchr,dcol
'    open cons for output as #1
    for dx = -5 to 5
        for dy = -5 to 5
            if maze.getseen(x+dx,y+dy,z) > 0 then
                dt = maze.gettile(x+dx,y+dy,z)
                dchr = TileCharTab(dt)
                dcol = TileColorTab(dt)
                select case dt
                case TILE_FLOOR
                    dchr = maze.floortile(z)
                    dcol = maze.floortilecolor(z)
                case TILE_WALL
                    dchr = maze.walltile(z)
                    dcol = maze.walltilecolor(z)
                case TILE_NOTHING    
                    dchr = maze.bordertile(z)
                    dcol = maze.bordertilecolor(z)
                end select
'                print #1,dt
                
                gfx.print (dx+5)*8,(dy+5)*8,chr(dchr),dcol,1
                if x+dx = player.x and y+dy = player.y and z = player.z then
                    gfx.printmc (dx+5)*8,(dy+5)*8,chr(player.curchar),0
                end if
            else
                gfx.print (dx+5)*8,(dy+5)*8,chr(dchr),0,1
            end if
        next dy    
    next dx
    
    'Show health
'    gfx.print 104,8,chr(3),9,1
'    gfx.print 112,8,str(player.health)+"    ",15,1
    'Show mana
'    gfx.print 104,8,chr(5),9,1
'    gfx.print 112,8,str(player.health)+"    ",15,1
    'Show current weapon power
'    gfx.print 104,8,chr(3),9,1
'    gfx.print 112,8,str(player.currentattack)+"    ",15,1
    
    gfx.show
end sub
