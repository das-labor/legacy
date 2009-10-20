OSTYPE          = $(firstword $(subst _, ,$(shell uname)))
ALLOWED_OSTYPES = Linux MINGW32 Darwin Freebsd CYGWIN

ifeq ($(findstring $(OSTYPE), $(ALLOWED_OSTYPES)),)
  $(error Unkown OSTYPE "$(OSTYPE)", must be one of "$(ALLOWED_OSTYPES)")
endif

ifeq ($(OSTYPE),MINGW32)
	INCLUDE = Makefile.win
else
ifeq ($(OSTYPE),Darwin)
    INCLUDE = Makefile.osx
else
    INCLUDE = Makefile.lin
endif
endif

-include $(INCLUDE)
