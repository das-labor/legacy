enum
    WALL_NORTH = 1
    WALL_EAST
    WALL_SOUTH
    WALL_WEST
end enum
#define BOXMAZESIZE 8
#define BOXMAZEGRIDSIZE 4
sub mazetype.createboxmaze(floor as integer)
    dim as integer x,y,d,tx,ty
    dim as integer exits,counter,temp,r,temp2
    dim as integer done
    open cons for output as #1
    'First we want all to be floor    
    for x = 0 to MAZESIZE
        for y = 0 to MAZESIZE
            tile(x,y,floor) = TILE_FLOOR
        next y
    next x    
    
    'Add some walls to create a 8x8 grid
    for x = BOXMAZEGRIDSIZE-1 to MAZESIZE-1 step BOXMAZEGRIDSIZE
        for y = 0 to MAZESIZE
            tile(x,y,floor) = TILE_WALL
        next y
    next x    

    for y = BOXMAZEGRIDSIZE-1 to MAZESIZE-1 step BOXMAZEGRIDSIZE
        for x = 0 to MAZESIZE
            tile(x,y,floor) = TILE_WALL
        next x
    next y    

    dim as integer mazevis(BOXMAZESIZE,BOXMAZESIZE)
    dim as integer mz(BOXMAZESIZE,BOXMAZESIZE,4)
    dim as integer mzdoorpos(BOXMAZESIZE,BOXMAZESIZE,4)

    'Prepare maze
    for x = 1 to BOXMAZESIZE
        for y = 1 to BOXMAZESIZE
            for d = 1 to 4
                mz(x,y,d) = 1
                mazevis(x,y) = 0
            next d
        next y
    next x

    
    do
        x = int(rnd*BOXMAZESIZE)+1
        y = int(rnd*BOXMAZESIZE)+1
    loop until mazevis(x,y) = 0
    mazevis(x,y) = 5
    'Create the maze with some sort of backtracking technique
    done = 0
    d = 0
    do
        r+=1
        'Did we have some unsuccsesfull tries?
        if r > 6 then
            r=0      
            d = mazevis(x,y)
            select case d
            case 1
                y -= 1
            case 2
                x += 1
            case 3
                y += 1
            case 4
                x -= 1
            case 5
                exit do
            end select        
        end if
        
        if r>2 then d = int(rnd*4)+1
        if (int(rnd*10)>5) then d = int(rnd*4)+1
'        print #1,x,y,d
        temp = int(rnd*3)-2
        temp2 = int(rnd*3)
        select case d
        case 1
            if y = 1 then continue do
            if mazevis(x,y-1)>0 then continue do
            mz(x,y,WALL_NORTH) = temp
            mzdoorpos(x,y,WALL_NORTH) = temp2
            y -=1
            mz(x,y,WALL_SOUTH) = temp
            mzdoorpos(x,y,WALL_SOUTH) = temp2
            mazevis(x,y) = 3
        case 2
            if x = BOXMAZESIZE then continue do
            if mazevis(x+1,y)>0 then continue do
            mz(x,y,WALL_EAST) = temp
            mzdoorpos(x,y,WALL_EAST) = temp2
            x +=1
            mz(x,y,WALL_WEST) = temp
            mzdoorpos(x,y,WALL_WEST) = temp2
            mazevis(x,y) = 4
        case 3
            if y = BOXMAZESIZE then continue do
            if mazevis(x,y+1)>0 then continue do
            mz(x,y,WALL_SOUTH) = temp
            mzdoorpos(x,y,WALL_SOUTH) = temp2
            y +=1
            mz(x,y,WALL_NORTH) = temp
            mzdoorpos(x,y,WALL_NORTH) = temp2
            mazevis(x,y) = 1
        case 4
            if x = 1 then continue do
            if mazevis(x-1,y)>0 then continue do
            mz(x,y,WALL_WEST) = temp
            mzdoorpos(x,y,WALL_WEST) = temp2
            x -=1
            mz(x,y,WALL_EAST) = temp
            mzdoorpos(x,y,WALL_EAST) = temp2
            mazevis(x,y) = 2
        end select
        
        dim as integer cx,cy,ct,cu
        ct = 0
        cu = 0
        
        for cx = 1 to BOXMAZESIZE
            for cy = 1 to BOXMAZESIZE
                ct += 1
                if mazevis(cx,cy) > 0 then cu = cu + 1
            next cy
        next cx
        
        if ct = cu then done = 1
        r = 0
            
    loop until done = 1

    if 1 = 1 then
    for x = 1 to BOXMAZESIZE
        for y = 1 to BOXMAZESIZE
            if mazevis(x,y) = 0 then
                do
                    d = int(rnd*4)+1
                    select case d
                    case 1
                        if y = 1 then continue do
                        if mazevis(x,y-1) >= 10 then continue do
                    case 2
                        if x = BOXMAZESIZE then continue do
                        if mazevis(x+1,y) >= 10 then continue do
                    case 3
                        if y = BOXMAZESIZE then continue do
                        if mazevis(x,y+1) >= 10 then continue do
                    case 4
                        if x = 1 then continue do
                        if mazevis(x-1,y) >= 10 then continue do
                    end select
                    exit do
                loop
                temp = int(rnd*2)-1
                select case d
                case 1
                    mz(x,y,WALL_NORTH) = temp
                    mz(x,y-1,WALL_SOUTH) = temp
                    mazevis(x,y) = 10
                case 2
                    mz(x,y,WALL_EAST) = temp
                    mz(x+1,y,WALL_WEST) = temp
                    mazevis(x,y) = 10
                case 3
                    mz(x,y,WALL_SOUTH) = temp
                    mz(x,y+1,WALL_NORTH) = temp
                    mazevis(x,y) = 10
                case 4
                    mz(x,y,WALL_WEST) = temp
                    mz(x-1,y,WALL_EAST) = temp
                    mazevis(x,y) = 10
                end select
            end if
        next y
    next x
    end if
    
    for x = 1 to BOXMAZESIZE
        for y = 1 to BOXMAZESIZE
            if mazevis(x,y) < 20 then
                for d = 1 to 4
                    if mz(x,y,d) = 0 then
                        temp = mzdoorpos(x,y,d)
                        select case d
                        case 1
                            tile(((x-1)*BOXMAZEGRIDSIZE)+temp               ,((y-1)*BOXMAZEGRIDSIZE)-1                ,floor) = TILE_DOOR
                        case 2
                            tile(((x-1)*BOXMAZEGRIDSIZE)+BOXMAZEGRIDSIZE-1  ,((y-1)*BOXMAZEGRIDSIZE)+temp             ,floor) = TILE_DOOR
                        case 3
                            tile(((x-1)*BOXMAZEGRIDSIZE)+temp               ,((y-1)*BOXMAZEGRIDSIZE)+BOXMAZEGRIDSIZE-1,floor) = TILE_DOOR
                        case 4
                            tile(((x-1)*BOXMAZEGRIDSIZE)-1                  ,((y-1)*BOXMAZEGRIDSIZE)+temp             ,floor) = TILE_DOOR
                        end select                
                    end if
                    if mz(x,y,d) < 0 then
                        select case d
                        case 1
                            for tx = 0 to 2
                                tile(((x-1)*BOXMAZEGRIDSIZE)+tx               ,((y-1)*BOXMAZEGRIDSIZE)-1                ,floor) = TILE_FLOOR
                            next tx
                        case 2
                            for tx = 0 to 2
                                tile(((x-1)*BOXMAZEGRIDSIZE)+BOXMAZEGRIDSIZE-1,((y-1)*BOXMAZEGRIDSIZE)+tx               ,floor) = TILE_FLOOR
                            next tx
                        case 3
                            for tx = 0 to 2
                                tile(((x-1)*BOXMAZEGRIDSIZE)+tx               ,((y-1)*BOXMAZEGRIDSIZE)+BOXMAZEGRIDSIZE-1,floor) = TILE_FLOOR
                            next tx
                        case 4
                            for tx = 0 to 2
                                tile(((x-1)*BOXMAZEGRIDSIZE)-1                ,((y-1)*BOXMAZEGRIDSIZE)+tx               ,floor) = TILE_FLOOR
                            next tx
                        end select                
                    end if
                next d
            else
                for tx = 0 to 2
                    for ty = 0 to 2
                        tile(((x-1)*BOXMAZEGRIDSIZE)+tx,((y-1)*BOXMAZEGRIDSIZE)+ty,floor) = TILE_WALL
                    next ty                
                next tx
            end if
        next y
    next x

    'Say what tiles this maze uses    
    walltile(floor) = 128
    walltilecolor(floor) = 9
    floortile(floor) = 129
    floortilecolor(floor) = 8
    bordertile(floor) = 128
    bordertilecolor(floor) = 1
        
end sub
