# Makefile.in generated automatically by automake 1.4-p6 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = ../..

prefix = /data/Data/projekte/DilloTor/tor-0.1.1.23/binary
exec_prefix = /data/Data/projekte/DilloTor/tor-0.1.1.23/binary

bindir = /data/Data/projekte/DilloTor/tor-0.1.1.23/binary/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/tor
pkglibdir = $(libdir)/tor
pkgincludedir = $(includedir)/tor

top_builddir = ../..

ACLOCAL = aclocal-1.4
AUTOCONF = autoconf
AUTOMAKE = automake-1.4
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = 
host_triplet = i686-pc-linux-gnu
BINDIR = /data/Data/projekte/DilloTor/tor-0.1.1.23/binary/bin
CC = gcc
CONFDIR = /data/Data/projekte/DilloTor/tor-0.1.1.23/binary/etc/tor
HAVE_LIB = @HAVE_LIB@
LIB = @LIB@
LOCALSTATEDIR = /data/Data/projekte/DilloTor/tor-0.1.1.23/binary/var
LOGFACILITY = 
LTLIB = @LTLIB@
MAKEINFO = /data/Data/projekte/DilloTor/tor-0.1.1.23/missing makeinfo
PACKAGE = tor
RANLIB = ranlib
TORGROUP = _tor
TORUSER = _tor
VERSION = 0.1.1.23

TESTS = test

noinst_PROGRAMS = test

bin_PROGRAMS = tor

tor_SOURCES = buffers.c circuitbuild.c circuitlist.c 	circuituse.c command.c config.c 	connection.c connection_edge.c connection_or.c control.c 	cpuworker.c directory.c dirserv.c dns.c hibernate.c main.c 	onion.c policies.c relay.c rendcommon.c rendclient.c rendmid.c 	rendservice.c rephist.c router.c routerlist.c routerparse.c 	tor_main.c


tor_LDADD = ../common/libor.a ../common/libor-crypto.a -lz -lssl -lcrypto

test_SOURCES = buffers.c circuitbuild.c circuitlist.c 	circuituse.c command.c config.c 	connection.c connection_edge.c connection_or.c control.c 	cpuworker.c directory.c dirserv.c dns.c hibernate.c main.c 	onion.c policies.c relay.c rendcommon.c rendclient.c rendmid.c 	rendservice.c rephist.c router.c routerlist.c routerparse.c 	test.c


test_LDADD = ../common/libor.a ../common/libor-crypto.a -lz -lssl -lcrypto

noinst_HEADERS = or.h
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../../orconfig.h
CONFIG_CLEAN_FILES = 
PROGRAMS =  $(bin_PROGRAMS) $(noinst_PROGRAMS)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I../..
CPPFLAGS = 
LDFLAGS = 
LIBS = -lpthread -ldl  -levent -lssl -lcrypto
tor_OBJECTS =  buffers.o circuitbuild.o circuitlist.o circuituse.o \
command.o config.o connection.o connection_edge.o connection_or.o \
control.o cpuworker.o directory.o dirserv.o dns.o hibernate.o main.o \
onion.o policies.o relay.o rendcommon.o rendclient.o rendmid.o \
rendservice.o rephist.o router.o routerlist.o routerparse.o tor_main.o
tor_DEPENDENCIES =  ../common/libor.a ../common/libor-crypto.a
tor_LDFLAGS = 
test_OBJECTS =  buffers.o circuitbuild.o circuitlist.o circuituse.o \
command.o config.o connection.o connection_edge.o connection_or.o \
control.o cpuworker.o directory.o dirserv.o dns.o hibernate.o main.o \
onion.o policies.o relay.o rendcommon.o rendclient.o rendmid.o \
rendservice.o rephist.o router.o routerlist.o routerparse.o test.o
test_DEPENDENCIES =  ../common/libor.a ../common/libor-crypto.a
test_LDFLAGS = 
CFLAGS = -g -O2 -Wall -g -O2
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
HEADERS =  $(noinst_HEADERS)

DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
SOURCES = $(tor_SOURCES) $(test_SOURCES)
OBJECTS = $(tor_OBJECTS) $(test_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu --include-deps src/or/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-binPROGRAMS:

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

distclean-binPROGRAMS:

maintainer-clean-binPROGRAMS:

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	done

mostlyclean-noinstPROGRAMS:

clean-noinstPROGRAMS:
	-test -z "$(noinst_PROGRAMS)" || rm -f $(noinst_PROGRAMS)

distclean-noinstPROGRAMS:

maintainer-clean-noinstPROGRAMS:

.c.o:
	$(COMPILE) -c $<

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

tor: $(tor_OBJECTS) $(tor_DEPENDENCIES)
	@rm -f tor
	$(LINK) $(tor_LDFLAGS) $(tor_OBJECTS) $(tor_LDADD) $(LIBS)

test: $(test_OBJECTS) $(test_DEPENDENCIES)
	@rm -f test
	$(LINK) $(test_LDFLAGS) $(test_OBJECTS) $(test_LDADD) $(LIBS)

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags -o $$here/TAGS $(ETAGS_ARGS) $$tags  $$unique $(LISP))

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = src/or

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
buffers.o: buffers.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
circuitbuild.o: circuitbuild.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
circuitlist.o: circuitlist.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h ../common/ht.h
circuituse.o: circuituse.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
command.o: command.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
config.o: config.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h ../common/aes.h ../common/ht.h \
	../common/test.h
connection.o: connection.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
connection_edge.o: connection_edge.c or.h ../../orconfig.h \
	../common/torint.h ../common/crypto.h ../common/tortls.h \
	../common/compat.h ../common/log.h ../common/container.h \
	../common/util.h ../common/torgzip.h
connection_or.o: connection_or.c or.h ../../orconfig.h \
	../common/torint.h ../common/crypto.h ../common/tortls.h \
	../common/compat.h ../common/log.h ../common/container.h \
	../common/util.h ../common/torgzip.h
control.o: control.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
cpuworker.o: cpuworker.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
directory.o: directory.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
dirserv.o: dirserv.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
dns.o: dns.c or.h ../../orconfig.h ../common/torint.h ../common/crypto.h \
	../common/tortls.h ../common/compat.h ../common/log.h \
	../common/container.h ../common/util.h ../common/torgzip.h \
	../common/ht.h
hibernate.o: hibernate.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
main.o: main.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
onion.o: onion.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
policies.o: policies.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
relay.o: relay.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
rendclient.o: rendclient.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
rendcommon.o: rendcommon.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
rendmid.o: rendmid.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
rendservice.o: rendservice.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
rephist.o: rephist.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
router.o: router.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
routerlist.o: routerlist.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
routerparse.o: routerparse.c or.h ../../orconfig.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h
test.o: test.c ../../orconfig.h or.h ../common/torint.h \
	../common/crypto.h ../common/tortls.h ../common/compat.h \
	../common/log.h ../common/container.h ../common/util.h \
	../common/torgzip.h ../common/test.h
tor_main.o: tor_main.c

check-TESTS: $(TESTS)
	@failed=0; all=0; \
	srcdir=$(srcdir); export srcdir; \
	for tst in $(TESTS); do \
	  if test -f $$tst; then dir=.; \
	  else dir="$(srcdir)"; fi; \
	  if $(TESTS_ENVIRONMENT) $$dir/$$tst; then \
	    all=`expr $$all + 1`; \
	    echo "PASS: $$tst"; \
	  elif test $$? -ne 77; then \
	    all=`expr $$all + 1`; \
	    failed=`expr $$failed + 1`; \
	    echo "FAIL: $$tst"; \
	  fi; \
	done; \
	if test "$$failed" -eq 0; then \
	  banner="All $$all tests passed"; \
	else \
	  banner="$$failed of $$all tests failed"; \
	fi; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$dashes"; \
	echo "$$banner"; \
	echo "$$dashes"; \
	test "$$failed" -eq 0
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
	$(MAKE) $(AM_MAKEFLAGS) check-TESTS
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am: install-binPROGRAMS
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-binPROGRAMS
uninstall: uninstall-am
all-am: Makefile $(PROGRAMS) $(HEADERS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(bindir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-binPROGRAMS mostlyclean-noinstPROGRAMS \
		mostlyclean-compile mostlyclean-tags \
		mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-binPROGRAMS clean-noinstPROGRAMS clean-compile \
		clean-tags clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-binPROGRAMS distclean-noinstPROGRAMS \
		distclean-compile distclean-tags distclean-generic \
		clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-binPROGRAMS \
		maintainer-clean-noinstPROGRAMS \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-binPROGRAMS distclean-binPROGRAMS clean-binPROGRAMS \
maintainer-clean-binPROGRAMS uninstall-binPROGRAMS install-binPROGRAMS \
mostlyclean-noinstPROGRAMS distclean-noinstPROGRAMS \
clean-noinstPROGRAMS maintainer-clean-noinstPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile tags mostlyclean-tags distclean-tags \
clean-tags maintainer-clean-tags distdir check-TESTS info-am info \
dvi-am dvi check check-am installcheck-am installcheck install-exec-am \
install-exec install-data-am install-data install-am install \
uninstall-am uninstall all-redirect all-am all installdirs \
mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
