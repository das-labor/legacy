#!/usr/bin/env python

import hashlib
import random
import MySQLdb
import csv
import datetime
import sys


# about config
host = "localhost"
user = "<user>"
passwd = "<password>"
db = "gnucash"

# die Kontoschluessel fuer die StandardRahmenKonten (SRK) haben sich evt
# im laufe der zeit geaendert. Hier ein Mapping fuer alle verwendeten Konten
# es muessen alle konten angegeben werden die verwendet wurden
# das Konto mit der nummer 10.000 (zen tausend) ist nicht Bestandteil der SRK
# wir verwenden es als std Ausgleichskonto - Hier geht alles hin was nicht
# zugeordnet werden soll - oder eben spaeter in gnucash

kontoersetzung = {  '0200':'0220',  '0330':'0330',
                    '0555':'0555',  '0920':'0920',
                    '2400':'2400',  '2664':'2664',
                    '2701':'2701',  '2702':'2702',
                    '2704':'2704',  '3215':'3215',
                    '3220':'3220',  '4901':'4901',
                    '6000':'6000',  '6001':'2110',
                    '6002':'4712',  '6003':'5702',
                    '6004':'3220',  '6300':'6300',
                    '6301':'6301',  '6302':'6302',
                    '6328':'6328',  '6329':'6329',
                    '6330':'6330',  '6331':'6331',
                    '6339':'6339',  '6353':'6353',
                    '6525':'6525',  '9000':'9000',
                    '10000':'10000', '2110':'2110',
                    '4712':'4712'  }

class Buchung(object):
  """
  Klasse zum halten der Information einer Buchung
  """
  datum = None
  # HabenKonto
  quellkontosrkcode = None
  quellkontoid = None
  #Sollkonto
  zielkontosrkcode = None
  zielkontoid = None
  eintragsdatum = None
  textbeschreibung = None
  waehrung = None
  waehrungid = None
  betrag = 0.0
  kurs = 1.0
  normbetrag = 0
  normkurs = 100
  guid = None
  defaultKonto = None
  defaultKontoID = None


  def __init__(self,connection):
    """
    wir erwarten hier eine initialisierte mysql connection
    """
    self.connection = connection
    self.__safeCreateDatum()
    self.guid = self.create32byteid()

  # es folgen setter
  
  def setDatum(self,datum):
    """
    erzeugt ein datimeobjekt fuer den string '20.02.2006'
    """
    #self.datum=datetime.datetime.strptime(datum, "%d.%m.%Y")
    # oder eben 20.02.06 die fuer die spaarkassen importe notwendig sind
    # es tritt hier das Problem auf, dass die csv evt nachbearbeitet werden muessen
    # da unter umstaenden fiktive Daten genommen wurde wie z.B. 29.02.11
    self.datum=datetime.datetime.strptime(datum, "%d.%m.%y")

  def setBetrag(self,betrag,kurs='1.0'):
    """
    hier wird der eigentlich Betrag der Buchung gesetzt. Gnucash verlangt von uns,
    dass wir nicht 42,23 als Betrag haben, sondern 4223. Ferner kommt eine Skalierung
    von der Spaarkasse, die wir auch beruecksichtigen.
    
    """
    # ersetze beim stringbetrag das komma durch einen punkt
    self.betrag = float(betrag.replace(",","."))
    self.kurs = float(kurs.replace(",","."))
    # float ist per definition nicht so genau -> echtes runden auf 2 Kommastellen genau
    self.normbetrag = round(self.betrag,2)
    # auf format ohne komme bringen
    self.normbetrag = round(self.normbetrag * self.normkurs,2)
    # den kurz beruecksichtigen
    self.normbetrag =  round(self.normbetrag / self.kurs,2)
    # und auf int runden
    self.normbetrag = int ( round(self.normbetrag,0))

  def setNormkurs(self,normkurs):
    """
    wird fuer die Berechnung des Betrages benoetigt
    """
    self.normkurs = normkurs

  def setCurrency(self,cur):
    """
    jede Buchung bringt eine Waehrung mit 
    """
    self.waehrung = cur
    # intern speichern wir auch gleich die entsprechende ID ab
    self.__fetchCurrencyID()

  def setTextbeschreibung(self,text):
    """
    der Text der Buchung - hier sollte Verwendungzweck und Gegenstelle auftauchen
    """
    self.textbeschreibung = text

  def getCurrencyID(self):
    return self.waehrungid
    
  def getDatum(self):
    return self.datum

  def __safeCreateDatum(self):
    """
    wann wurde die Buchung erstellt wird hier abgespeichert
    """
    self.eintragsdatum = datetime.datetime.now()

  def __fetchCurrencyID(self):
    """
    die Buchung hat eine Waerung. Fuer diese Waehrung muss es in der Datenbank einen Eintrag geben.
    Dieser Eintrag hat eine guid die 128Bit lang ist. Dies guid brauchen wir um es in der Buchung zu
    hinterlegen.
    """
    cursor = self.connection.cursor ()
    # mysql statement ausfuehren
    cursor.execute ("SELECT * from commodities where namespace=\"CURRENCY\" and mnemonic=\""+self.waehrung+"\";")
    # wir erwarten nur einen Eintrag
    row = cursor.fetchone ()
    cursor.close ()
    self.waehrungid = row[0]


  def createTransactionInput(self):
    """
    Als erstes speichern wir eine buchung ab - diese enthaelt gerade mal das Datum der Buchung
    und den Betreff - der Betrag wird ueber splits geregelt
    """
    # 
    #  insert into transactions(guid,currency_guid,post_date,enter_date,description)
    #    values (self.guid,self.waehrungid,self.datum,self.textbeschreibung)
    
    tmp = ""
    tmp = tmp + "insert into transactions(guid,currency_guid,post_date,enter_date,description) "
    tmp = tmp + "values("
    tmp = tmp + "\"" + str(self.guid) + "\","
    self.__fetchCurrencyID()
    tmp = tmp + "\"" + str(self.waehrungid) + "\","
    tmp = tmp + "\"" + str(self.datum).split(" ")[0] + "\","
    tmp = tmp + "\"" + str(self.eintragsdatum).split(".")[0] + "\","
    tmp = tmp + "\"" + str(self.textbeschreibung) + "\");"
    # wir machen keine Aenderungen an der Datenbank, sondern geben
    # einfach nur das konstruierte SQL-Statement zurueck
    return tmp

  def createSplitInputs(self):
    """
    Eine Buchung wird in Teile zerlegt - Dabei geht das geld von einem Konto zu einem anderem.
    Z.B. jemand ueberweisst 50 Euro auf das Laborkonto - Davon sollen 20 Euro Mitgliedsbeitrag sein
    und 30 Euro fuer die Getraenke Kasse. Initial ist alles 
    """

    tmp = "insert into splits(guid,tx_guid,account_guid,action,reconcile_state,reconcile_date,value_num,value_denom,quantity_num,quantity_denom) "
    tmp = tmp + "values( "
    tmp = tmp + "\""+str(self.create32byteid()) + "\","
    tmp = tmp + "\""+str(self.guid) + "\","
    tmp = tmp + "\""+str(self.quellkontoid) + "\","
    tmp = tmp + "\"c\","
    tmp = tmp + "\"Deposit\","
    tmp = tmp + "\"" + str(self.eintragsdatum).split(".")[0] + "\","
    tmp = tmp + "\"" + str( int( self.normbetrag)) + "\","
    tmp = tmp + "\"" + str(self.normkurs)+ "\","
    tmp = tmp + "\"" + str( int( self.normbetrag)) + "\","
    tmp = tmp + "\"" + str(self.normkurs)+ "\");\n"

    tmp = tmp + "insert into splits(guid,tx_guid,account_guid,reconcile_state,value_num,value_denom,quantity_num,quantity_denom) "
    tmp = tmp + "values( "
    tmp = tmp + "\""+str(self.create32byteid()) + "\","
    tmp = tmp + "\""+str(self.guid) + "\","
    tmp = tmp + "\""+str(self.zielkontoid) + "\","
    tmp = tmp + "\"n\","
    tmp = tmp + "\"" + str(-1 * int( self.normbetrag)) + "\","
    tmp = tmp + "\"" + str(self.normkurs)+ "\","
    tmp = tmp + "\"" + str(-1 * int( self.normbetrag)) + "\","
    tmp = tmp + "\"" + str(self.normkurs)+ "\");\n"
    return tmp

  def create32byteid(self):
    return hashlib.md5(str(random.randint(0,100000000000000000))).hexdigest()

  def checkKontoID(self,code):
    cursor = self.connection.cursor ()
    cursor.execute ("SELECT * from accounts where code=\""+code+"\";")
    row = cursor.fetchone ()
    cursor.close ()
    if row != None:
      return True
    return False
    

  def __fetchkontoID(self,code):
    cursor = self.connection.cursor ()
    cursor.execute ("SELECT * from accounts where code=\""+code+"\";")
    row = cursor.fetchone ()
    cursor.close ()
    if row != None:
      return row[0]
    else:
      print "did not find code: " + str(code) 
      return self.defaultKontoID

  def __fetchDefaultKonto(self,name):
    cursor = self.connection.cursor ()
    cursor.execute ("SELECT * from accounts where name=\""+name+"\";")
    row = cursor.fetchone ()
    cursor.close ()
    return row[0]


  def setDefaultKonto(self,ktnname):
    self.defaultKonto = ktnname
    self.defaultKontoID = self.__fetchDefaultKonto(ktnname)

  def setQuellkontoCode(self,code):
    self.quellkontosrkcode = code
    self.quellkontoid = self.__fetchkontoID(code)

  def setZielkontoCode(self,code):
    self.zielkontosrkcode = code
    self.zielkontoid = self.__fetchkontoID(code)



def getCSVFile(filename):
  return csv.reader(open(filename,"U"),delimiter=";")


db = sys.argv[1]

conn = MySQLdb.connect (host = host,
                        user = user,
                        passwd = passwd,
                        db = db)






if sys.argv[2] == "check" :
  # check for accounts
  for i in getCSVFile(sys.argv[3]):
    entry = Buchung(conn)
    if not entry.checkKontoID(kontoersetzung[i[0]]):
      print "could not find konto " + kontoersetzung[i[0]]
    if not entry.checkKontoID(kontoersetzung[i[1]]):
      print "could not find konto " + kontoersetzung[i[1]]
elif sys.argv[2] == "create" :
  for i in getCSVFile(sys.argv[3]):
    entry = Buchung(conn)
    entry.setDefaultKonto("Ausgleichskonto-EUR")
    entry.setDatum(i[3])
    entry.setCurrency(i[10])
    entry.setTextbeschreibung(i[5] + " " + i[6])
    entry.setQuellkontoCode(kontoersetzung[i[0]])
    entry.setZielkontoCode(kontoersetzung[i[1]])
    entry.setBetrag(i[9],i[11])
    print str(entry.createTransactionInput())
    print str(entry.createSplitInputs())

conn.close ()





