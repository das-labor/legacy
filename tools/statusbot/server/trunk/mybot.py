#!/usr/bin/env python


# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


from jabberbot import JabberBot,botcmd
import datetime
import random
import os
import sys


class SystemInfoJabberBot(JabberBot):

    NODATA = "waiting for data - please make a call"
    CLOSED = "geschlossen"
    OPEN = "offen"

    # where are my images?
    www_nodata="/srv/www.das-labor.org/htdoc/statusNONE.png"
    www_closed="/srv/www.das-labor.org/htdoc/statusOFF.png"
    www_open="/srv/www.das-labor.org/htdoc/statusON.png"
    # what is the destination of that image
    www_show="/srv/www.das-labor.org/htdoc/status.png"

    #datetime of the last statusupdate
    dt_lastmessage=datetime.datetime(1,1,1,0,0, 0, 0)
    @botcmd
    def mauz(self,mess,args):
        """react on schnittchen"""
        response=['wwaaahhhh Kratzenhaare!!!',
                  '/me bindet die Kratze ans Channeltopic',
                  '/me klebt ne Gummiemaus an die Decke',
                  '/me verteilt Katzenminze im Channel',
                  'kommm kitty kitty kommm ... ich hab da was feines',
                  '/me hebt das Schild "Niedliche Katze zu verschenken!" hoch',
                  'KATZLA!',
                  'http://www.cuisine.at/rezept_0886450_katze_in_sahne_auf_polnische_art.php']
        if mess.getFrom() == "labor@conference.das-labor.org/channelkatze":
            return str(random.choice(response))

    @botcmd
    def help(self,mess,args):
        return None

    @botcmd
    def kratze(self,mess,args):
        """whats a kratze"""
        return "Jeder Buchling kennt Hildegunst von Mythenmetz fabelhaften Roman ueber Echo, das Kraetzchen - eine zamonische Kratze unterscheidet sich uebrigens von unserer Katze lediglich dadurch, dass sie sprechen kann: In Sledwaya, der traurigsten Stadt Zamoniens, ist Echo dermassen in Schwierigkeiten geraten, dass er gezwungen ist, mit dem Schrecksenmeister Succubius Eisspin einen verhaengnisvollen Vertrag zu schliessen."


    # need to override callback for keepalive
    def callback_keepalive(self):
        dt_now=datetime.datetime.now()
        if ((dt_now-self.dt_lastmessage).seconds > 120) or ((dt_now-self.dt_lastmessage).days != 0):
            self.set_status(self.NODATA)
            self.set_show(self.AWAY)
            os.system("cp "+str(self.www_nodata)+" "+str(self.www_show))
        self._send_status()

    @botcmd
    def setoffen(self,mess,args):
        """what to do if signal for labor open arrives"""
        if mess.getFrom() == "statsetter@das-labor.org/JabberBot":        
            self.dt_lastmessage=datetime.datetime.now()
            self.set_status(self.OPEN)
            self.set_show(self.AVAILABLE)
            os.system("cp "+str(self.www_open)+" "+str(self.www_show))
        # change other files hier

    @botcmd
    def setzu(self,mess,args):
        """what to do if signal for labor closed arrives"""
        if mess.getFrom() == "statsetter@das-labor.org/JabberBot":
            self.dt_lastmessage=datetime.datetime.now()
            self.set_status(self.CLOSED)
            self.set_show(self.DND)
            os.system("cp "+str(self.www_closed)+" "+str(self.www_show))
        # change other files hier


def connect_callback():
        # set default status
        bot.set_show(bot.AWAY)
        bot.set_status(bot.NODATA)


username = 'status@das-labor.org/statusdaemon'
password = 'password'
bot = SystemInfoJabberBot(username,password)
#register some commands
bot.commands["*mauz*"]=bot.mauz
bot.commands["*maunz*"]=bot.mauz
bot.commands["mauz!"]=bot.mauz
bot.commands["maunz!"]=bot.mauz
bot.commands["maunz"]=bot.mauz
bot.commands["miau"]=bot.mauz
bot.commands["miau!"]=bot.mauz
bot.commands["*miau*"]=bot.mauz
bot.commands["kratze?"]=bot.kratze


bot.join_room("labor@conference.das-labor.org",username="#ChanServ")
#bot.send("admin@conference.das-labor.org","foo",message_type='groupchat')
bot.serve_forever(connect_callback)
