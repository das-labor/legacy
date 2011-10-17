#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(lap-host, 0.0.1)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR(src/cand/main.c)
AM_CONFIG_HEADER(config.h)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_ISC_POSIX
AC_PROG_LIBTOOL

# Checks for libraries.
#AC_CHECK_LIB([ezV24], [v24Getc], [], AC_MSG_ERROR([libezV24 not found]) )

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
# AC_FUNC_MALLOC
# AC_TYPE_SIGNAL

## AC_CONFIG_FILES([Makefile lap-dump/Makefile])
AC_CONFIG_FILES([
Makefile
src/Makefile
src/liblap/Makefile
])

AC_OUTPUT
