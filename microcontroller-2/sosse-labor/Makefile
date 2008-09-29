# $Id: Makefile,v 1.2 2002/12/24 13:23:13 m Exp $

all:
	(cd src ; make)
	(cd doxygen ; doxygen sosse.cnf ; cd latex ; make ; make refman.ps)

all-emu:
	(cd src ; make -f Makefile.emu)
	(cd doxygen ; doxygen sosse.cnf ; cd latex ; make ; make refman.ps)

clean:
	(cd src ; make clean)
	(cd doxygen ; rm -rf html latex)

clean-emu:
	(cd src ; make -f Makefile.emu clean)
	(cd doxygen ; rm -rf html latex)

