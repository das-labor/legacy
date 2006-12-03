#!/usr/bin/env python
from xml.dom import minidom, Node
import urllib

sock = urllib.urlopen("http://www.das-labor.org/termine.xml")
dom = minidom.parse(sock)

termine = []

def gat(obj,attr):
    return obj.getElementsByTagName(attr)[0].firstChild.data

def texify(s):
    return s
    

class Termin:
    def __init__(self):
        pass

    def __str__(self):
        return "%s %s.%s.%s %s:%s %s %s %s" % (self.wday, self.day, self.month, self.year, self.hour, self.min, self.text, self.link, self.match)

    def tex(self):
        return "{\\noindent\\vbox{\\underline{\\textbf{%s}: \\textit{%s}}}%s\\vskip 4mm}" % ( self.day + "." + self.month + ".", self.hour + ":" + self.min, texify(self.text))



for termin in dom.getElementsByTagName("termin"):
    #print termin.toxml()
    t = Termin()
    t.month = gat(termin,"month")
    t.text = gat(termin,"text")
    t.hour = gat(termin,"hour")
    t.day = gat(termin,"day")
    t.year = gat(termin,"year")
    t.min = gat(termin,"min")
    t.link = gat(termin,"link")
    t.wday = gat(termin,"wday")
    t.match = gat(termin,"match")
    termine.append(t)

for termin in termine:
    print termin


