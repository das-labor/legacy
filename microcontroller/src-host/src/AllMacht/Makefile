SRC=Lap.cs Main.cs
PRG=allmacht.exe
LIBS=-pkg:glade-sharp -pkg:glade-sharp -resource:gui.glade


all:${SRC}
	mcs ${LIBS} -out:${PRG} ${SRC}

clean:
	rm -f *.exe

install:
	cp allmacht.exe /usr/bin
	
