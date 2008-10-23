
#define DEBUG 0
declare sub Intro()
#include "helper.bi"
#include "gfx.bi"
dim shared gfx as gfxtype
#include "gamemain.bi"


gfx.loadcharset "font.bmp"
gfx.loadmccharset "mcchars.bmp"
randomize timer
'gfx.testpicture()
Intro()
sleep
gfx.clear()
NewGame()


do:loop until inkey = ""
'sleep

sub Intro()
    dim as integer x,y,c,t,t2
    dim as integer showstuff
    dim as string logostringtab(1 to 5)
    dim as integer logoxtab(1 to 5)
    logostringtab(1) = "8-BIT-ROGUE"
    logostringtab(2) = "by"
    logostringtab(3) = "MARKUS STEHR"
    logostringtab(4) = "for the"
    logostringtab(5) = "RR-COMPO '08"

    for t = 1 to 5
        logoxtab(t) = ((SCREENSIZE_X+1)-(len(logostringtab(t))*8))/2
    next t



    showstuff = 1
    for t2 = 1 to 300
        for t = 1 to 100
            x = int(rnd*(SCREENSIZE_X+1))
            y = int(rnd*(SCREENSIZE_Y+1))
            c = int(rnd*14)+1
            gfx.draw(x,y,c)
        next t
        if inkey <> "" then showstuff = 0
        if showstuff = 1 then
            gfx.show
            sleep 1,1
        end if
    next t2


    for t2 = 1 to 200
        for t = 1 to 1000
            x = int(rnd*(SCREENSIZE_X-16))+8
            y = int(rnd*(SCREENSIZE_Y-16))+8
            gfx.draw(x,y,0)
        next t
        if inkey <> "" then showstuff = 0
        if showstuff = 1 then
            gfx.show
            sleep 1,1
        end if
    next t2
    for t2 = 1 to ubound(logostringtab)
        for t = 0 to 2
            gfx.print logoxtab(t2),4+(t2*8),logostringtab(t2),gfx.fadetab(t),0
            if inkey <> "" then showstuff = 0
            if showstuff = 1 then
                gfx.show
                sleep 200,1
            end if
        next t
    next t2



    gfx.show

end sub



