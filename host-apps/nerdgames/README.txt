
### EULA 
du verpflichtest dich durch die benutzung des Programms anderen Leuten spass zu bringen
und der GPLv2.0 (oder groesser) unter der diese Software steht folge zu leisten.

Fragen, Sorgen, Noete und Probleme bitte an: ask le pios ( at. ) das-labor.org


### Anleitung
# configuration
es koennen nur bilder und sounds abgespielt werden die sich im unterordner 'media' befinden. 
Dort muessen die Dateien nur einfach mit Leben befuellt werden. im Moment wird aplay zum abspielen
der wavs verwendet (playsound routine im pythonfile) - dies ist gegebenen falls anzupassen. 
Analog fuer das beenden des Tons (killsound)

# steuerung
es koennen bis zu vier spieler mitspielen die initial mit einem Namen versehen werden muessen. Nach 
dem starten wird nur das hintergrund bild angezeigt. Die Taseten 1,2,3,4 aktivieren den spieler und 
verlangen nach den Namen des Spielers ( es kann auch mit nur 2 spielern gespielt werden)

sind alle Spieler vorhanden, wird das Spiel durch das druecken der 'x' taste gestartet.

mit der maus werden die einzelnen Spielflaechen aktiviert.

die spieler schalten sich hinzu durch das druecken ihrer Taste:
spieler1: j
spieler2: g
spieler3: i
spieler4: h
(Achtung casesensitive)

mit den tasten 'r' fuer richtige Antwort und 'w' fuer falsche antwort kann der moderator die punkte vergeben

kann kein spieler die frage beantworten kann der moderator durch druecken der Taste 'd' die runde beenden und 
niemand bekommt punkte

wurde eine frage durch einen spieler falsch beantwortet, koennen die anderen weiter antworten. durch druecken
der taste 'a' kann der moderator den sound noch einmal abspielen. Dies geht auch wenn der sound beendet wurde 
und einfach noch mal abgespielt werden soll. 


Das spielfeld (playfield.avg) muss evt angepasst werden - gerade was ueberschriften der einzelnen thematas angeht. 

### libavg
fuer das spiel wird die libavg von www.libavg.org benoetigt. Um diese zu bauen war es zumindest unter meinem system
notwendig ein symlink von /usr/lib64/libpython2.7.so nach /usr/lib64/libpython2.7.dylib anzulegen.


# TODO
i) gibt kein speichern - wenn das programm sich beendet, dann ist es einfach tot und hat alles vergessen
ii) risiko-games : der spieler darf entscheiden wie viele Punkte er setzen moechte
iii) sekt oder selters fuer einzelne spieler
iv) point catcher: "ich packe meinen koffer und nehme mit" jede richtige iteration gibt 50 Punkte fuer den spieler 
 der dran ist - die spielrunde dauert so lange bis _kein_ spieler mehr uebrig ist

### nodes

# umwandeln einer flv-datei in wav
mplayer -ao pcm:fast:file=audio.wav -vo null -vc null NmCCQxVBfyM.flv 
