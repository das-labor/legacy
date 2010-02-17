#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Author: Marcel Fourné
# License: MIT-X11
#######################

from jabberbot import JabberBot
import socket, threading
from shutil import copy

(JID, PASSWORD) = ('statusv2@das-labor.org','tellmeapassword')
HOST = ''
PORT = 5233

class StatusBotv2(JabberBot):
    """Klasse für den Labor JabberBot, welcher den Status des Labors anzeigen soll. 
    Hat ein Socket-Interface, über das eine "1" das Labor als offen markiert, eine "0" als geschlossen."""

    def __init__( self, jid, password, res = None):
        super( StatusBotv2, self).__init__( jid, password, res)
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.bind((HOST, PORT))
        self.s.listen(1)
        self.thread_killed = False
  
    def thread_proc( self):
        copy("StatusNONE.png", "status.png")
        self.status_message = "Laborstatus unbekannt"
        self.status_type = self.DND
        # manuelles handling: 
        # wenn programm beendet wird wartet der thread immer noch auf IO, 
        # also 1 mal IO schicken, damit die manuelle logik greifen kann
        while not self.thread_killed:
            conn, addr = self.s.accept()
            while 1:
                data = conn.recv(1024)
                if not data: break
                if data.find("1") == 0:
                    copy("StatusON.png", "status.png")
                    self.status_message = "Labor ist offen"
                    self.status_type = self.CHAT
                elif data.find("0") == 0:
                    copy("StatusOFF.png", "status.png")
                    self.status_message = "Labor ist geschlossen"
                    self.status_type = self.XA
                else:
                    copy("StatusNONE.png", "status.png")
                    self.status_message = "Laborstatus unbekannt"
                    self.status_type = self.DND
                    conn.close()
                if self.thread_killed:
                    return

lb = StatusBotv2(JID, PASSWORD)
th = threading.Thread( target = lb.thread_proc)
# daemon-mode für den thread bringt es nicht, weil auf IO wartende threads nicht beendet werden können
th.daemon = True
# Haupt-Schleife starten mit Thread anbei für die Arbeit
lb.serve_forever(connect_callback = lambda: th.start())
# manuelles handling...
lb.thread_killed = True
