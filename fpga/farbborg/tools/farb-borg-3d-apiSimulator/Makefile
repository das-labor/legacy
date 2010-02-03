OSTYPE          = $(firstword $(subst _, ,$(shell uname)))
ALLOWED_OSTYPES = Linux MINGW32 Darwin Freebsd CYGWIN

ifeq ($(findstring $(OSTYPE), $(ALLOWED_OSTYPES)),)
  $(error Unkown OSTYPE "$(OSTYPE)", must be one of "$(ALLOWED_OSTYPES)")
endif

#PROBLEMS on WINDOWS??? read this
#get following fils windows if you have problems with compiling
#http://sourceforge.net/projects/mingw/files/MSYS%20coreutils/coreutils-5.97-2/coreutils-5.97-2-msys-1.0.11-bin.tar.lzma/download
#http://sourceforge.net/projects/mingw/files/MSYS%20Base%20System/msys-1.0.11/msysCORE-1.0.11-msys-1.0.11-base-bin.tar.lzma/download
#and install it into the mingw folder of codeblocks. Add the path of mingw/bin to the PATH envirementvariable.
#and install the glut libary. (http://www.das-labor.org/files/madex/glut.3.7.6_mingw_codeblocks.zip)
#make a copy of mingw/bin/mingw32-make.exe with the name mingw/bin/make.exe

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
