#!/usr/bin/env python

from jabberbot import JabberBot
import random
import sys
import os
import time


# what is my account
username = 'statsetter@das-labor.org/status'
# what is the password for that account
password = ''
# what is the account which should receive the messages
remotebot='status@das-labor.org'
# time between two updates in seconds
sleeptime=60
# we are working via ping - so what should i ping?
# default sould be slotmachine (printerserver)
pinghost="10.0.1.10"
# command to send to the remote bot if everything is fine
cmdfine="setoffen"
# command to send to the remote bot if encountering a problem
cmdfail="setzu"


########### END CONFIG

tmp=os.system("ping -c 1 "+pinghost)
bot = JabberBot(username,password)
if tmp == 0:
    bot.send(remotebot,cmdfine)
else:
    bot.send(remotebot,cmdfail)



