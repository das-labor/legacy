allg: wir gehen davon aus, dass gnucash seine Daten in einer
mysql-datenbank ablegt.

massimporter.py db command file.csv
 db: die datenbank in die eingetragen werden soll
 command: < check | create >  
   check: ueberpruefen, ob alle in der csv verwendeten 
          Konten vorhanden sind
   create: erstelle sql-insert commands auf stdout

die meisten einstellungen wie benutzername und datenbankserver sind
direkt in dem script zu machen.

das format der csv-datei laesst sich natuerlich recht leicht
anpassen. das aktuell eingestellt sieht etwa so aus:

"Soll";"Haben";"ignore";"eingangsdatum";"ignore";"Betreff 1";"Betreff 2";"ignore";"ignore";"Betrag";"waehrung";"skalierung"



import_suite:
 $ ./csvspkbo2yamls.rb sparkassebochum.csv > sparkassebochum.yaml
 konvertiert das sparkassen-csv in eine yaml-datei
 $ ./yamls_to_gcfriendly_csv.rb sparkassebochum.yaml > gc-friendly-import.csv
 konvertiert eine yaml-datei in eine csv-datei die besser in gnucash
 zu importieren ist
 $ ./persist_yamls.rb sparkassebochum.yaml
 die eintraege in der yaml-datei werden in einer sqlite-datenbank
 abgelegt die von ./csvspkbo2yamls.rb beruecksichtigt wird. So koennen
 doppelte eintraege vermieden werden.


Ablauf:
 1) download csv-datei von sparkasse - nennen wir sie sparkasse.csv
 2) virtualbox starten
 3) die VM gnucash starten
 4) sparkasse.csv in die VM bringen
 5) cd import_suite
 6) ./csvspkbo2yamls.rb sparkasse.csv > sparkasse.yaml
 7) ./yamls_to_gcfriendly_csv.rb sparkasse.yaml > gcf-sparkasse.csv
 8) startx
 9) Datei -> Importieren -> CSV
10) gcf-sparkasse.csv auswaehlen
11) ein Popup kommt hoch und fragt nach dem standardkonto. dies ist
 immer die Kasse mit der Nummer 0920
12) die einzelnen Buchungen den entsprechenden Konten zuordnen - egal
 was, es handelt sich immer um eine neue Buchung, nie um einen
 Ausgleich.
13) wenn nicht klar ist, zu welchem konto etwas gehoert, dann erstemal
 ins Ausgleichskonto-EUR - d.h. soviel wie "schau ich mir spaeter noch
 mal an"
14) gnucash beenden 
15) X beenden (logout)
16) ./persist_yamls.rb sparkasse.yaml
17) sudo poweroff



Anm.: 
 im LaborOrga SVN liegt unter
 /documents/allg.Verein/DATEV-KontenrahmenVereine/ ein pdf-dokument
 mit den aktuellen StandardRahmenKonto Nummern. Es bietet sich an dort
 nach Sinnvollen Konten zu suchen.

