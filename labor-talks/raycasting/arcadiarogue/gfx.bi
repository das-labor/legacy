#define SCREENSIZE_X  160-1
#define SCREENSIZE_Y  120-1

#define PIXELSIZE_X 640/(SCREENSIZE_X+1)
#define PIXELSIZE_Y 480/(SCREENSIZE_Y+1)

type gfxtype
    screenpuffer(0 to SCREENSIZE_X,0 to SCREENSIZE_Y) as integer
    screenpufferdrawn(0 to SCREENSIZE_X,0 to SCREENSIZE_Y) as integer
    colortab(0 to 15) as unsigned integer

    fadetab(0 to 3) as integer

    drawstartx as integer
    drawstarty as integer
    declare sub movescreen(x as integer, y as integer)
    declare sub resetscreencoords(mode as integer)

    declare function getcolor(x as integer,y as integer) as integer

    declare sub draw(x as integer,y as integer,c as integer)
    declare sub clear()
    declare sub show()
    charset(0 to 7,0 to 7,0 to 255) as byte
    declare sub loadcharset(f as string)

    mccharset(0 to 7,0 to 7,0 to 255) as byte
    declare sub loadmccharset(f as string)

    declare sub print(x as integer, y as integer, s as string, c as integer,mode as integer)
    declare sub printmc(x as integer, y as integer, s as string,mode as integer)

    messages(1 to 4) as string
    declare sub AddToMessages(s as string)
    declare sub WriteMessages()

    declare sub testpicture()

    declare constructor
end type

'Our constructor to set up the GFX code
constructor gfxtype
    'Go graphic mode 640x480 with 32 bit colors
    screenres 640,480,32

    'Set up the color table
    colortab(0) = rgb(  0,  0,  0)
    colortab(1) = rgb(127,  0,  0)
    colortab(2) = rgb(  0,127,  0)
    colortab(3) = rgb(  0,  0,127)
    colortab(4) = rgb(127,  0,127)
    colortab(5) = rgb(  0,127,127)
    colortab(6) = rgb(127,127,  0)
    colortab(7) = rgb(127,127,127)
    colortab(8) = rgb( 63, 63, 63)
    colortab(9) = rgb(240,  0,  0)
    colortab(10) = rgb(  0,240, 10)
    colortab(11) = rgb( 10,  0,240)
    colortab(12) = rgb(220, 10,220)
    colortab(13) = rgb(  0,240,240)
    colortab(14) = rgb(240,240,  0)
    colortab(15) = rgb(240,240,240)

    fadetab(0) = 8
    fadetab(1) = 7
    fadetab(2) = 15
    fadetab(3) = 15
end constructor

sub gfxtype.AddToMessages(s as string)
    dim t as integer
    messages(1) = messages(2)
    messages(2) = messages(3)
    messages(3) = messages(4)
    messages(4) = s

    for t = 1 to 4
        messages(t) += string(22-len(messages(t))," ")
    next t


    WriteMessages()
end sub

sub gfxtype.WriteMessages()
    dim t as integer
    for t = 1 to 4
        print 0,(10*8)+(t*8),messages(5-t),fadetab(4-t),1
    next t
    show
end sub

'Load in a font/charset
sub gfxtype.loadcharset(f as string)
    dim as any ptr tempbmp
    dim as integer x,y,c
    tempbmp = imagecreate(8*256,8,0)
    bload f,tempbmp
    for c = 0 to 255
        for x = 0 to 7
            for y = 0 to 7
                if point(x+(c*8),y,tempbmp) = rgb(0,0,0) then
                    charset(x,y,c) = 0
                else
                    charset(x,y,c) = 1
                end if
            next y
        next x
    next c

    imagedestroy tempbmp
end sub

'Load in a multicolor font/charset
sub gfxtype.loadmccharset(f as string)
    dim as any ptr tempbmp
    dim as integer x,y,c
    dim as long pt
'    open cons for output as #1
    tempbmp = imagecreate(8*256,8,0)
    bload f,tempbmp
    for c = 0 to 255
        for x = 0 to 7
            for y = 0 to 7
                pt = point(x+(c*8),y,tempbmp)
                'print #1, pt
                select case pt
                case rgb(  0,  0,  0)
                    mccharset(x,y,c) = 0
                case rgb(127,  0,  0)
                    mccharset(x,y,c) = 1
                case rgb(  0,127,  0)
                    mccharset(x,y,c) = 2
                case rgb(  0,  0,127)
                    mccharset(x,y,c) = 3
                case rgb(127,  0,127)
                    mccharset(x,y,c) = 4
                case rgb(  0,127,127)
                    mccharset(x,y,c) = 5
                case rgb(127,127,  0)
                    mccharset(x,y,c) = 6
                case rgb(127,127,127)
                    mccharset(x,y,c) = 7
                case rgb( 63, 63, 63)
                    mccharset(x,y,c) = 8
                case rgb(240,  0,  0)
                    mccharset(x,y,c) = 9
                case rgb(  0,240, 10)
                    mccharset(x,y,c) = 10
                case rgb( 10,  0,240)
                    mccharset(x,y,c) = 11
                case rgb(220, 10,220)
                    mccharset(x,y,c) = 12
                case rgb(  0,240,240)
                    mccharset(x,y,c) = 13
                case rgb(240,240,  0)
                    mccharset(x,y,c) = 14
                case rgb(240,240,240)
                    mccharset(x,y,c) = 15
                case else
                    mccharset(x,y,c) = 0
                end select
            next y
        next x
    next c

    imagedestroy tempbmp
'    close #1
end sub

'Draws a string to designated position
sub gfxtype.print(x as integer, y as integer, s as string, c as integer,mode as integer)
    dim as integer cx,cy,dx,dy,sp,slen,curchar

    slen = len(s)
    dx = x
    dy = y
    'Doing this outside the loop speeds things up, but doubles the code... ;)
    if mode = 0 then
        for sp = 1 to slen
            curchar = asc(mid(s,sp,1))
            for cx = 0 to 7
                for cy = 0 to 7
                    if charset(cx,cy,curchar) > 0 then draw(dx+cx,dy+cy,c)
                next cy
            next cx
            dx += 8
        next sp
    else
        for sp = 1 to slen
            curchar = asc(mid(s,sp,1))
            for cx = 0 to 7
                for cy = 0 to 7
                    if charset(cx,cy,curchar) > 0 then
                        draw(dx+cx,dy+cy,c)
                    else
                        draw(dx+cx,dy+cy,0)
                    end if
                next cy
            next cx
            dx += 8
        next sp
    end if
end sub

'Draws a string to designated position with the multi color font
sub gfxtype.printmc(x as integer, y as integer, s as string,mode as integer)
    dim as integer cx,cy,dx,dy,sp,slen,curchar

    slen = len(s)
    dx = x
    dy = y
    'Doing this outside the loop speeds things up, but doubles the code... ;)
    if mode = 0 then
        for sp = 1 to slen
            curchar = asc(mid(s,sp,1))
            for cx = 0 to 7
                for cy = 0 to 7
                    if mccharset(cx,cy,curchar) > 0 then draw(dx+cx,dy+cy,mccharset(cx,cy,curchar))
                next cy
            next cx
            dx += 8
        next sp
    else
        for sp = 1 to slen
            curchar = asc(mid(s,sp,1))
            for cx = 0 to 7
                for cy = 0 to 7
                    draw(dx+cx,dy+cy,mccharset(cx,cy,curchar))
                next cy
            next cx
            dx += 8
        next sp
    end if
end sub


' Just some code to test the GFX code
sub gfxtype.testpicture()
    dim as integer x,y
    dim as double t
    for x = 0 to SCREENSIZE_X
        for y = 0 to SCREENSIZE_Y
            draw(x,y,int(x/10))
        next y
    next x
    show
    sleep
    if inkey = chr(27) then exit sub
    t = timer
    do
        movescreen(1,0)
        show
        do:sleep 1:loop until timer > t+.1
        t = timer
    loop until inkey = chr(27)

    resetscreencoords(1)

    print 0,0,"Test abcABC",15,0
    print 0,8,"Test abcABC",15,1
    show


end sub

'Changes the screen position variables
sub gfxtype.movescreen(x as integer, y as integer)
    drawstartx += x
    drawstarty += y
    if drawstartx < 0 then drawstartx += SCREENSIZE_X
    if drawstartx > SCREENSIZE_X then drawstartx -= SCREENSIZE_X
    if drawstarty < 0 then drawstarty += SCREENSIZE_Y
    if drawstarty > SCREENSIZE_Y then drawstarty -= SCREENSIZE_Y
end sub

sub gfxtype.resetscreencoords(mode as integer)
    dim puf(0 to SCREENSIZE_X, 0 to SCREENSIZE_Y) as integer
    dim as integer x,y
    if mode = 1 then
        for x = 0 to SCREENSIZE_X
            for y = 0 to SCREENSIZE_Y
                puf(x,y) = getcolor(x,y)
            next y
        next x
        drawstartx = 0
        drawstarty = 0
        for x = 0 to SCREENSIZE_X
            for y = 0 to SCREENSIZE_Y
                draw(x,y,puf(x,y))
            next y
        next x
        exit sub
    end if
    drawstartx = 0
    drawstarty = 0
end sub

' Draw a single pixel
sub gfxtype.draw(x as integer,y as integer,c as integer)
    if x < 0 or x > SCREENSIZE_X or y < 0 or y > SCREENSIZE_Y then exit sub
    screenpuffer(x,y) = c and 15
end sub

'Gives back the color, according to the screen position variables
function gfxtype.getcolor(x as integer,y as integer) as integer
    dim as integer gx,gy
    gx = drawstartx + x
    gy = drawstarty + y
    if gx > SCREENSIZE_X then gx -= SCREENSIZE_X
    if gy > SCREENSIZE_Y then gy -= SCREENSIZE_Y
    return screenpuffer(gx,gy)
end function

'Show the screen
'Will only update what has been changed
sub gfxtype.show()
    dim as integer x,y
'    screensync
    screenlock
    for x = 0 to SCREENSIZE_X
        for y = 0 to SCREENSIZE_Y
            if getcolor(x,y) <> screenpufferdrawn(x,y) then
                line(x*PIXELSIZE_X                ,y*PIXELSIZE_Y)- _
                    ((x*PIXELSIZE_X)+PIXELSIZE_X-1,(y*PIXELSIZE_Y)+PIXELSIZE_Y-1) _
                    ,colortab(getcolor(x,y)),bf

                screenpufferdrawn(x,y) = getcolor(x,y)
            end if
        next y
    next x
    screenunlock
end sub

'Just clears the whole screen
sub gfxtype.clear()
    dim as integer x,y
    for x = 0 to SCREENSIZE_X
        for y = 0 to SCREENSIZE_Y
            screenpuffer(x,y) = 0
        next y
    next x
end sub

