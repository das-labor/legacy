OSTYPES = beos cygwin freebsd linux mingw mac
ifeq ($(findstring $(OSTYPE), $(OSTYPES)),)
  $(error Unkown OSTYPE "$(OSTYPE)", must be one of "$(OSTYPES)")
endif

ifeq ($(OSTYPE),mingw)
	INCLUDE = Makefile.win
else
ifeq ($(OSTYPE),mac)
    INCLUDE = Makefile.mac
else
    INCLUDE = Makefile.lin
endif
endif

-include $(INCLUDE)
