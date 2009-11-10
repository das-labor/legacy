# Makefile for building lm32 toolchain (newlib based)
#
# By Martin Ongsiek

# Target
TARGET = lm32-elf
#PWD = /Users/madex2/Develop/lm32_4.4

# Directories
SOURCES_DIR := ${PWD}/sources
BUILD_DIR := ${PWD}/build
PATCHES_DIR := ${PWD}/patches
PREFIX := ${BUILD_DIR}/${TARGET}

#HOSTINCLUDE := /opt/local/include
#HOSTLIB := /opt/local/lib

# Source versions

VERSION_BINUTILS = 2.20
VERSION_GCC = 4.4.0
VERSION_GDB = 7.0
VERSION_NEWLIB = 1.17.0

VERSION_GMP = 4.3.1
VERSION_MPFR = 2.4.1

# Tool flags
MKDIR_FLAGS = -p
RM_FLAGS = -rf
WGET_FLAGS = -nc
TAR_FLAGS_BZ2 = xjf
TAR_FLAGS_GZ = xzf
TAR_FLAGS_LZMA = xv --lzma -f
BUNZIP2_FLAGS = -kf

# Add toolchain to path
PATH := ${PREFIX}/bin:${PATH}

# Default target
#all:  gcc-final
all: download extract patches binutils gcc newlib 

dirs:
	mkdir ${MKDIR_FLAGS} ${SOURCES_DIR}
	mkdir ${MKDIR_FLAGS} ${BUILD_DIR}
	mkdir ${MKDIR_FLAGS} ${PATCHES_DIR}
	mkdir ${MKDIR_FLAGS} ${PREFIX}
	touch dirs

download: dirs
	wget ${WGET_FLAGS} -P ${SOURCES_DIR} ftp://gcc.gnu.org/pub/binutils/releases/binutils-${VERSION_BINUTILS}.tar.bz2
	wget ${WGET_FLAGS} -P ${SOURCES_DIR} ftp://gcc.gnu.org/pub/gcc/releases/gcc-${VERSION_GCC}/gcc-core-${VERSION_GCC}.tar.bz2
	wget ${WGET_FLAGS} -P ${SOURCES_DIR} ftp://gcc.gnu.org/pub/gcc/releases/gcc-${VERSION_GCC}/gcc-g++-${VERSION_GCC}.tar.bz2
	wget ${WGET_FLAGS} -P ${SOURCES_DIR} ftp://gcc.gnu.org/pub/gcc/releases/gcc-${VERSION_GCC}/gcc-testsuite-${VERSION_GCC}.tar.bz2
	wget ${WGET_FLAGS} -P ${SOURCES_DIR} ftp://sourceware.org/pub/gdb/releases/gdb-${VERSION_GDB}.tar.bz2
	wget ${WGET_FLAGS} -P ${SOURCES_DIR} ftp://sources.redhat.com/pub/newlib/newlib-${VERSION_NEWLIB}.tar.gz
	wget ${WGET_FLAGS} -P ${PATCHES_DIR} http://gcc.gnu.org/ml/gcc-patches/2008-12/msg01024/lm32.patch
	wget ${WGET_FLAGS} -P ${SOURCES_DIR} ftp://ftp.gmplib.org/pub/gmp-${VERSION_GMP}/gmp-${VERSION_GMP}.tar.bz2
	wget ${WGET_FLAGS} -P ${SOURCES_DIR} http://www.mpfr.org/mpfr-current/mpfr-${VERSION_MPFR}.tar.bz2
	touch download

extract:
	tar ${TAR_FLAGS_BZ2} ${SOURCES_DIR}/binutils-${VERSION_BINUTILS}.tar.bz2 -C ${BUILD_DIR}
	tar ${TAR_FLAGS_BZ2} ${SOURCES_DIR}/gcc-core-${VERSION_GCC}.tar.bz2 -C ${BUILD_DIR}
	tar ${TAR_FLAGS_BZ2} ${SOURCES_DIR}/gcc-g++-${VERSION_GCC}.tar.bz2 -C ${BUILD_DIR}
	tar ${TAR_FLAGS_BZ2} ${SOURCES_DIR}/gcc-testsuite-${VERSION_GCC}.tar.bz2 -C ${BUILD_DIR}
	tar ${TAR_FLAGS_BZ2} ${SOURCES_DIR}/gdb-${VERSION_GDB}.tar.bz2 -C ${BUILD_DIR}
	tar ${TAR_FLAGS_GZ} ${SOURCES_DIR}/newlib-${VERSION_NEWLIB}.tar.gz -C ${BUILD_DIR}
	tar ${TAR_FLAGS_BZ2} ${SOURCES_DIR}/gmp-${VERSION_GMP}.tar.bz2 -C ${BUILD_DIR}
	mv ${BUILD_DIR}/gmp-${VERSION_GMP} ${BUILD_DIR}/gcc-${VERSION_GCC}/gmp
	tar ${TAR_FLAGS_BZ2} ${SOURCES_DIR}/mpfr-${VERSION_MPFR}.tar.bz2 -C ${BUILD_DIR}
	mv ${BUILD_DIR}/mpfr-${VERSION_MPFR} ${BUILD_DIR}/gcc-${VERSION_GCC}/mpfr
	touch extract

patches: 
	cd ${BUILD_DIR}/gcc-${VERSION_GCC}; \
	patch -p0 < ${PATCHES_DIR}/lm32.patch; \
	touch patches

binutils:
	mkdir ${BUILD_DIR}/build-binutils; \
	cd ${BUILD_DIR}/build-binutils; \
	../binutils-${VERSION_BINUTILS}/configure --target=${TARGET} --prefix=${PREFIX} \
	--with-gcc \
	--with-gnu-as \
	--with-gnu-ld \
	--with-dwarf2 --disable-werror  \
	--disable-nls; \
	make all-bfd TARGET-bfd=headers; \
	rm bfd/Makefile; \
	make configure-bfd; \
	make; \
	make install; \
	cd ../../
	touch binutils

# Note: --disable-werror is added to avoid compile break by warning (eg. ubuntu gcc is too restrictive)

# Bootstrap gcc (C only compiler)
gcc: binutils.complete
	mkdir ${BUILD_DIR}/build-gcc; \
	cd ${BUILD_DIR}/build-gcc; \
	../gcc-${VERSION_GCC}/configure --target=${TARGET} --prefix=${PREFIX} \
	--enable-languages=c \
	--disable-libssp \
	--enable-sjlj-exceptions \
	--disable-nls \
	--with-newlib \
	--disable-libstdcxx-pch; \
	make; \
	make install; \
	cd ../../
	touch gcc
	
#	--with-gmp-include=$(HOSTINCLUDE) \
#  	--with-gmp-lib=$(HOSTLIB) \
#	--enable-languages=c,c++ fails! (why?)
#	cd ${BUILD_DIR}/gcc-${VERSION_GCC}; \
#	./configure --target=${TARGET} --prefix=${PREFIX} \
#	--disable-nls --disable-shared --disable-threads \
#	--disable-libssp --disable-libgomp --with-newlib \
#	--enable-languages="c" --disable-libmudflap; \
#	make; \
#	make install

newlib:
	mkdir ${BUILD_DIR}/build-newlib; \
	cd ${BUILD_DIR}/build-newlib; \
	../newlib-${VERSION_NEWLIB}/configure --target=${TARGET} --prefix=${PREFIX}; \
	make; \
	make install; \
	cd ../../
	touch newlib

gcc-final:
	mkdir ${BUILD_DIR}/build-gcc-final; \
	cd ${BUILD_DIR}/build-gcc-final; \
	../gcc-${VERSION_GCC}/configure --target=${TARGET} --prefix=${PREFIX} \
	--disable-nls --disable-shared --disable-threads \
	--disable-libssp \
	--enable-languages="c,c++" \
	--enable-sjlj-exceptions \
	--with-newlib; \
	make; \
	make install; \
	cd ../../
	touch gcc-final

gdb:
	mkdir ${BUILD_DIR}/build-gdb; \
	cd ${BUILD_DIR}/build-gdb; \
	../gdb-${VERSION_GDB}/configure --target=${TARGET} --prefix=${PREFIX} \
	#cd ${BUILD_DIR}/gdb-${VERSION_GDB}; \
	#./configure --target=${TARGET} --prefix=${PREFIX} \
	--disable-nls --disable-werror; \
	make all-bfd TARGET-bfd=headers; 
	make; \
	make install; \
	cd ../../
	touch gdb

# Fails on missing target bfd support! (configure: error: *** unknown target vector bfd_elf32_avr32_vec)

distclean: clean
	rm ${RM_FLAGS} ${SOURCES_DIR}

clean:
	rm ${RM_FLAGS} ${BUILD_DIR}
	rm ${RM_FLAGS} ${PATCHES_DIR}

.PHONY: all

