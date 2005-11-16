SRC=AssemblyInfo.cs Main.cs Display.cs
PRG=StyroDesign.exe
LIBS=-pkg:glade-sharp -pkg:glade-sharp -resource:gui.glade


all:${SRC}
	mcs ${LIBS} -out:${PRG} ${SRC}

clean:
	rm -f *.exe


