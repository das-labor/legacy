#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(lap-host, 0.1)
AC_CONFIG_SRCDIR(lap-dump/main.c)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lezV24':
AC_CHECK_LIB([ezV24], [main])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_TYPE_SIGNAL

## AC_CONFIG_FILES([Makefile lap-dump/Makefile])
AC_OUTPUT([Makefile lap-dump/Makefile])
