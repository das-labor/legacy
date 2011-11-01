#!/usr/bin/env python
# -*- coding: utf-8 -*-

from libavg import *
from libavg import anim
import os
import random
from time import sleep
username=["alfred","bert","carl","detlef"]
neednames=0

player = avg.Player.get()
player.setResolution(True,1024,768,16)
player.loadFile("playfield.avg")
rootNode = player.getRootNode()
currentRound=""
currentPlayer=""
gotnames=0


q100=['thema1_100_div','thema2_100_div','thema3_100_div','thema4_100_div']
q200=['thema1_200_div','thema2_200_div','thema3_200_div','thema4_200_div']
q300=['thema1_300_div','thema2_300_div','thema3_300_div','thema4_300_div']
q400=['thema1_400_div','thema2_400_div','thema3_400_div','thema4_400_div']
q500=['thema1_500_div','thema2_500_div','thema3_500_div','thema4_500_div']





c=[]
c.extend(q100)
c.extend(q200)
c.extend(q300)
c.extend(q400)
c.extend(q500)

points={
    'user1' : {
        'score': 0,'name' :'alfred','active': True,'plays': False
        },
    'user2' : {
        'score': 0,'name' :'bert','active': True,'plays': False
        },
    'user3' : {
        'score': 0,'name' :'carl','active': True,'plays': False
        },
    'user4' : {
        'score': 0,'name' :'detlef','active': True,'plays': False
        }
    }

scores={
    'thema1_100' : 100, 'thema1_200' : 200, 'thema1_300' : 300, 'thema1_400' : 400, 'thema1_500' : 500,
    'thema2_100' : 100, 'thema2_200' : 200, 'thema2_300' : 300, 'thema2_400' : 400, 'thema2_500' : 500,
    'thema3_100' : 100, 'thema3_200' : 200, 'thema3_300' : 300, 'thema3_400' : 400, 'thema3_500' : 500,
    'thema4_100' : 100, 'thema4_200' : 200, 'thema4_300' : 300, 'thema4_400' : 400, 'thema4_500' : 500,
    }

    
    
def playSound(Filename):
    bException = 0
    p = 1
    if os.path.exists("/usr/bin/aplay"):
        while not(bException) and p > 0:
            try:
                p, status = os.waitpid(-1, os.WNOHANG)
            except OSError:
                bException = 1
        id = os.fork()
        if (id == 0):
#            os.execl("/usr/bin/mplayer", "mplayer", "-really-quiet","-ao","pulse", "media/"+Filename)
            os.execl("/usr/bin/aplay", "aplay", "-qN", "media/"+Filename)
            exit(0)

def killSound():
    if os.path.exists("/usr/bin/killall"):
        os.system("/usr/bin/killall aplay")
    
    

def readusername1(event):
    global player
    rootNode=player.getRootNode()
    if event.keystring == "left shift":
        return
    if event.keystring == "return":
        rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, keyboardFunc)
        playSound("keyboard_done.wav")
        user1doneObj.start()
        return
    elif event.keystring == "backspace":
        username[0]=username[0][0:len(username[0])-1]
        playSound("keyboard_back.wav")
    else:
        username[0] = username[0] + event.keystring
        playSound("keyboard_in.wav")
    
    player.getElementByID("user1_name").text=username[0]

    
def readusername2(event):
    global player
    rootNode=player.getRootNode()
    if event.keystring == "left shift":
        return
    if event.keystring == "return":
        rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, keyboardFunc)
        playSound("keyboard_done.wav")
        user2doneObj.start()
        return
    elif event.keystring == "backspace":
        username[1]=username[1][0:len(username[1])-1]
        playSound("keyboard_back.wav")
    else:
        username[1] = username[1] + event.keystring
        playSound("keyboard_in.wav")
    player.getElementByID("user2_name").text=username[1]


def readusername3(event):
    global player
    rootNode=player.getRootNode()
    if event.keystring == "left shift":
        return
    if event.keystring == "return":
        rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, keyboardFunc)
        playSound("keyboard_done.wav")
        user3doneObj.start()
        return
    elif event.keystring == "backspace":
        username[2]=username[2][0:len(username[2])-1]
        playSound("keyboard_back.wav")
    else:
        username[2] = username[2] + event.keystring
        playSound("keyboard_in.wav")
    
    player.getElementByID("user3_name").text=username[2]

def readusername4(event):
    global player
    rootNode=player.getRootNode()
    if event.keystring == "left shift":
        return
    if event.keystring == "return":
        rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, keyboardFunc)
        playSound("keyboard_done.wav")
        user4doneObj.start()
        return
    elif event.keystring == "backspace":
        username[3]=username[3][0:len(username[3])-1]
        playSound("keyboard_back.wav")
    else:
        username[3] = username[3] + event.keystring
        playSound("keyboard_in.wav")
    
    player.getElementByID("user4_name").text=username[3]

        

def keyboardFunc(event):
    global player
    global neednames
    global gotnames
    global points
    if gotnames != 0:
        if event.keystring == 'x':
            neednames=1
            playSound('Welcome_To_My_Domain.wav')
            fieldreadyanimObj.start()

    if event.keystring == 'left shift':
        return
    if neednames==0:
        if event.keystring == '1':
            gotnames=1
            points['user1']['plays']=True
            player.getElementByID("user1_div").active = True
            user1animObj.start()
            rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, readusername1)
        if event.keystring == '2':
            points['user2']['plays']=True
            gotnames=1
            player.getElementByID("user2_div").active = True
            user2animObj.start()
            rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, readusername2)
        if event.keystring == '3':
            points['user3']['plays']=True
            player.getElementByID("user3_div").active = True
            gotnames=1
            user3animObj.start()
            rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, readusername3)
        if event.keystring == '4':
            points['user4']['plays']=True
            gotnames=1
            player.getElementByID("user4_div").active = True
            user4animObj.start()
            rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, readusername4)
    reactivate_playfield()
        


def mouseovereffect(event):
    node=event.node.getElementByPos((0,0))
    if event.node.active == True:
        playSound("blob.wav")
        node.fillcolor="AAAAAA"

def mouseouteffect(event):
    node=event.node.getElementByPos((0,0))
    if event.node.active == True:
        node.fillcolor="888888"

def setuserpoints():
    global points
    global player
    for i in points:
        tmpnode=player.getElementByID(i+'_score')
        tmpnode.text=str(points[i]['score'])
        if points[i]['score'] > 0:
            tmpnode.color="00FF00"
        elif points[i]['score'] < 0:
            tmpnode.color="FF0000"

def decission(event):
    global player
    global points
    
    if event.keystring == 'r':
        playSound("rightAns.wav")
        tmpnode = player.getElementByID("startPlay");
        for i in points:
            if points[i]['name'] == currentPlayer:
                points[i]['score'] = points[i]['score'] + scores[currentRound]

        tmpnode.text="+"+str(scores[currentRound])
        tmpnode.color='00FF00'
        endroundanimObj.start()
        tmpnode=player.getElementByID("lockfield")
        setuserpoints()
        tmpnode=player.getElementByID(currentRound+"_div")
        tmpnode.opacity=0.1
        tmpnode.fillopacity=0.1
        tmpnode.active=False
        for i in points:
            points[i]['active']=points[i]['plays']
        pass

    if event.keystring == 'w':
        playSound("wrongAns.wav")
        tmpnode = player.getElementByID("startPlay");
        for i in points:
            if points[i]['name'] == currentPlayer:
                points[i]['score'] = points[i]['score'] - scores[currentRound]

        tmpnode.text="-"+str(scores[currentRound])
        tmpnode.color='FF0000'
        setuserpoints()
        failroundAanimObj.start()
        pass



def nouserinput(event):
    pass


    

def playround(event):
    global player
    global currentPlayer
    global points
    if event.keystring == 'a':
        killSound()
        playSound(currentRound+".wav")
    if event.keystring == 'd':
        killSound()
        tmpnode = player.getElementByID("startPlay");
        tmpnode.text="TTL exceed..."
        endroundanimObj.start()
        tmpnode=player.getElementByID(currentRound+"_div")
        tmpnode.opacity=0.1
        tmpnode.fillopacity=0.1
        tmpnode.active=False
        for i in points:
            points[i]['active']=points[i]['plays']

        
    if points['user1']['active']:
        if event.keystring == 'j':
            tmpnode=player.getElementByID("user1_name")
            currentPlayer=tmpnode.text
            player.getElementByID("startPlay").text=currentPlayer
            tmpnode=player.getElementByID("startPlay")
            tmpnode.active = True
            tmpnode.text=currentPlayer
            tmpnode.fontsize=80.0
            tmpnode.fillopacity=1.0
            tmpnode.opacity=1.0
            tmpnode.color='0000FF'
            killSound()
            playSound("usersounds/user1.wav")
            points['user1']['active']=False
            tmpnode.text=currentPlayer
            rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, decission)
    if points['user2']['active']:
        if event.keystring == 'g':
            tmpnode=player.getElementByID("user2_name")
            currentPlayer=tmpnode.text
            tmpnode=player.getElementByID("startPlay")
            tmpnode.active = True
            tmpnode.text=currentPlayer
            tmpnode.fontsize=80.0
            tmpnode.fillopacity=1.0
            tmpnode.opacity=1.0
            tmpnode.color='0000FF'
            killSound()
            playSound("usersounds/user2.wav")
            points['user2']['active']=False
            rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, decission)
    if points['user3']['active']:
        if event.keystring == 'i':
            tmpnode=player.getElementByID("user3_name")
            currentPlayer=tmpnode.text
            tmpnode=player.getElementByID("startPlay")
            tmpnode.active = True
            tmpnode.text=currentPlayer
            tmpnode.fontsize=80.0
            tmpnode.fillopacity=1.0
            tmpnode.opacity=1.0
            tmpnode.color='0000FF'
            killSound()
            playSound("usersounds/user3.wav")
            points['user3']['active']=False
            rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, decission)
    if points['user4']['active']:
        if event.keystring == 'h':
            tmpnode=player.getElementByID("user4_name")
            currentPlayer=tmpnode.text
            tmpnode=player.getElementByID("startPlay")
            tmpnode.active = True
            tmpnode.text=currentPlayer
            tmpnode.fontsize=80.0
            tmpnode.fillopacity=1.0
            tmpnode.opacity=1.0
            tmpnode.color='0000FF'
            killSound()
            playSound("usersounds/user4.wav")
            points['user4']['active']=False
            rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, decission)

random.seed()
def mouseupeffect(event):
    global player
    global currentRound
    global c
    active_counter=0
    active_specials=[]

    #tmpvalue=random.randrange(0,active_counter + len(active_specials) )
    # do special
    #    if tmpvalue > active_counter:#

    print len(active_specials)
    #print tmpvalue
    print active_counter
#    if len(active_specials) > 0:
#        if tmpvalue < len(active_specials)+1:
#            print active_specials
#            tmpnode_rand = random.choice(active_specials).getElementByPos((0,0))
#            currentRound=tmpnode_rand.id
#            playSound(tmpnode_rand.id + "_special.wav")
#            tmpnode_rand.active=False
#            tmpnode_rand.opacity=0.0
#            tmpnode_rand.fillopacity=0.0
#            tmpnode2=player.getElementByID(currentRound + '_text')
#            print currentRound + '_text'
#            tmpnode2.opacity=0.0
#            tmpnode=player.getElementByID("startPlay")
#            tmpnode.text="special"
#            tmpnode.fontsize=22.0
#            tmpnode.color='0000AA'
#            tmpnode.opacity=1.0
#            tmpnode=player.getElementByID("fullfill")
#            tmpnode.fillopacity=0.8
#            tmpnode=player.getElementByID("lockfield")
#            tmpnode.active=True
#            tmpnode=player.getElementByID("hintimage")
#            tmpnode.href="media/"+currentRound+"_hint.tif"
#            tmpnode.opacity=1.0
#            tmpnode.active=True
#            tmpnode3 = player.getElementByID(tmpnode_rand.id + '_div')
#            tmpnode3.active=False
#        
#            rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, playround_special)
#            return

    tmpnode=event.node.getElementByPos((0,0))
    currentRound=tmpnode.id
    playSound(tmpnode.id + ".wav")
    print "media/"+currentRound+"_hint.tif"
    if os.access("media/"+currentRound+".tif",os.R_OK):
        tmpnode=player.getElementByID("hintimage")
        tmpnode.href="media/"+currentRound+".tif"
        tmpnode.opacity=1.0
        tmpnode.active=True
    else:
        tmpnode=player.getElementByID("startPlay")
        tmpnode.text="listen!"
        tmpnode.fontsize=22.0
        tmpnode.color='0000AA'
        tmpnode.opacity=1.0
        tmpnode=player.getElementByID("hintimage")
        tmpnode.opacity=0.0
        tmpnode=player.getElementByID("fullfill")
        tmpnode.fillopacity=0.8
    tmpnode=player.getElementByID("lockfield")
    tmpnode.active=True
    event.node.active=False
    rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, playround)


    

#    rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, playround)


# initiale animationen

## a) move field to ready-postion
r=[]


thematas=['thema1','thema2','thema3','thema4']

# endround animation
def reactivate_playfield():
    global player
    global points
    tmpnode=player.getElementByID("lockfield")
    tmpnode.active=False
    for i in points:
        points[i]['active']=points[i]['plays']

endroundanimObj = avg.ParallelAnim([avg.LinearAnim(player.getElementByID("fullfill"),"fillopacity",500,0.8,0),
                                    avg.LinearAnim(player.getElementByID("startPlay"),"opacity",1500,1,0)])
endroundanimObj.setStopCallback(reactivate_playfield)

# animationen wenn falsche antwort - fade out points - bring in text
def roundretry():
    global player
    tmpnode=player.getElementByID("startPlay")
    tmpnode.text="retry"
    tmpnode.fontsize=22.0
    tmpnode.color='0000AA'
    failroundBanimObj.start()

def roundretryevthandl():
    global rootNode
    rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, playround)

failroundAanimObj = avg.LinearAnim(player.getElementByID("startPlay"),"opacity",1000,1,0)
failroundAanimObj.setStopCallback(roundretry)
failroundBanimObj = avg.LinearAnim(player.getElementByID("startPlay"),"opacity",500,0,1)
failroundBanimObj.setStopCallback(roundretryevthandl)

#roundspecialanimObj = avg.LinearAnim(player.getElementByID("startPlay"),"opacity",250,1,0)

#onmouseover effect
for i in c:
    tmpblock=player.getElementByID(i)
    tmpblock.setEventHandler(avg.CURSOROVER, avg.MOUSE, mouseovereffect)
    tmpblock.setEventHandler(avg.CURSOROUT, avg.MOUSE, mouseouteffect)
    tmpblock.setEventHandler(avg.CURSORUP, avg.MOUSE, mouseupeffect)

for i in thematas:
    r.append(LinearAnim(player.getElementByID(i),"opacity",1500,0,1))

#r.append(LinearAnim(player.getElementByID('thema5_200_div'),"x",1500,20,236))
#r.append(LinearAnim(player.getElementByID('thema5_300_div'),"x",1500,20,492))
#r.append(LinearAnim(player.getElementByID('thema5_400_div'),"x",1500,20,748))

for i in q100:
    r.append(LinearAnim(player.getElementByID(i),"y",1500,0,60))

for i in q200:
    r.append(LinearAnim(player.getElementByID(i),"y",1500,0,120))

for i in q300:
    r.append(LinearAnim(player.getElementByID(i),"y",1500,0,180))

for i in q400:
    r.append(LinearAnim(player.getElementByID(i),"y",1500,0,240))

for i in q500:
    r.append(LinearAnim(player.getElementByID(i),"y",1500,0,300))

fieldreadyanimObj = ParallelAnim(r)


## b) usernames animationen
user1animObj = ParallelAnim([LinearAnim(player.getElementByID("user1_div"),"y",500,0,300),
                              LinearAnim(player.getElementByID("user1_div"),"x",500,0,500),
                              LinearAnim(player.getElementByID("user1_div"),"opacity",500,0,1)])
user1doneObj = ParallelAnim([LinearAnim(player.getElementByID("user1_div"),"y",500,300,600),
                             LinearAnim(player.getElementByID("user1_div"),"x",500,500,64),
                             LinearAnim(player.getElementByID("user1_score"),"opacity",500,0,1)])

user2animObj = ParallelAnim([LinearAnim(player.getElementByID("user2_div"),"y",500,0,300),
                              LinearAnim(player.getElementByID("user2_div"),"x",500,0,500),
                              LinearAnim(player.getElementByID("user2_div"),"opacity",500,0,1)])
user2doneObj = ParallelAnim([LinearAnim(player.getElementByID("user2_div"),"y",500,300,600),
                             LinearAnim(player.getElementByID("user2_div"),"x",500,500,320),
                             LinearAnim(player.getElementByID("user2_score"),"opacity",500,0,1)])


user3animObj = ParallelAnim([LinearAnim(player.getElementByID("user3_div"),"y",500,0,300),
                              LinearAnim(player.getElementByID("user3_div"),"x",500,0,500),
                              LinearAnim(player.getElementByID("user3_div"),"opacity",500,0,1)])
user3doneObj = ParallelAnim([LinearAnim(player.getElementByID("user3_div"),"y",500,300,600),
                             LinearAnim(player.getElementByID("user3_div"),"x",500,500,576),
                             LinearAnim(player.getElementByID("user3_score"),"opacity",500,0,1)])


user4animObj = ParallelAnim([LinearAnim(player.getElementByID("user4_div"),"y",500,0,300),
                              LinearAnim(player.getElementByID("user4_div"),"x",500,0,500),
                              LinearAnim(player.getElementByID("user4_div"),"opacity",500,0,1)])

user4doneObj = ParallelAnim([LinearAnim(player.getElementByID("user4_div"),"y",500,300,600),
                             LinearAnim(player.getElementByID("user4_div"),"x",500,500,832),
                             LinearAnim(player.getElementByID("user4_score"),"opacity",500,0,1)])



## main keyevent handler
rootNode.setEventHandler(avg.KEYDOWN, avg.NONE, keyboardFunc)


# start player
player.play()

