
lib_LTLIBRARIES = liblap.la

liblap_la_SOURCES = cann.c liblap.c debug.c 
bin_PROGRAMS = canctrl cand

canctrl_LDADD = liblap.la
canctrl_LDFLAGS = @LDFLAGS@
canctrl_SOURCES = canctrl.c

cand_LDADD = liblap.la
cand_LDFLAGS = @LDFLAGS@
cand_SOURCES = cand.c uart-host.c can-uart.c 
