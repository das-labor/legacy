==Anleitung==

1) Lade die .ics-Dateien fuer Restmuell und Papiermuell von der USB-Seite herunter und lege sie unter den Dateinamen
USB_Restmuell.ics und USB_Blaumuell.ics (Papier) in diesem Verzeichnis ab. 
(http://www.usb-bochum.de/buerger/Leerungstermine.php)
2) Gehe auf die REMONDIS-Seite (http://www.remondis.de/dsd-bochum/ - vorsicht die suche ist arsch es muss genau 
nach "alleestrasse" gesucht werden ohne leerzeichen oder hausnummern) mit den "gelber Sack"-Terminen fuer das 
Jahr und speichere sie ab. Identifiziere das innerste <div>-Element, in dem die Abholtermine stehen und uebertrage 
es in eine Datei remondis.html.snippet
2a) Modifiziere das Makefile, auf dass es die Jahreszahl fuer die REMONDIS-Termine enthaelt
3) Hoffe, dass sich die Formate nicht wesentlich geaendert haben und rufe "make" auf.
4) Importiere muelltermine.ics in dein Kalenderprogramm
5) Ueberpruefe, ob die angezeigten Termine sinnvoll erscheinen.
6) checke die datei ins weblab als muell.ics ein und update das weblab. Die neuen Termine sollten unter LabStatus zu 
finden sein.

==Hintergruende==

Die beiden Skripte (siehe das makefile fuer den Aufruf) parsen die Termine und generieren daraus ein .ics bzw. einen
Abschnitt einer .ics-Datei. Die woechentlich wiederkehrenden Termine in den beiden input-ics werden in einzelne Termine
verwandelt, alle Termine werden auf den Tag zuvor verlegt als ganztaegige Termine, und mit einer in den Skripten
konfigurierbaren "Summary" versehen.

Sollten sich die Formate aendern, muessen die Skripte angepasst werden (ICS wurde nur ganz rudimentaer implementiert, fuer die
Webseite wird durch Substitution /<br>/<br\/>/ halbwegs valides XML generiert und einem Standard-XML-Parser uebergeben.
Die beiden Skripte koennen wie folgt getrennt gedebuggt werden:
* das ICS-Skript erwartet den output des HTML/XML-Skripts in einer Datei. Diese einfach mit touch leer anlegen, und der output des
  Skripts enthaelt nur die Daten, die aus den USB ICS-Dateien kommen.
* das HTML/XML-Skript operiert getrennt vom anderen, aus dem output erhaelt man ein valides .ics durch einen standard
  header und footer, den man im anderen Skript leicht erkennt.

Die Input-Dateien, mit denen die Skripte urspruenglich funktioniert haben, liegen im Verzeichnis Inputs_2010.
