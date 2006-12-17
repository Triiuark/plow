#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([plow], [0.0.2])

AM_INIT_AUTOMAKE([-Werror foreign])

AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADER([src/config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_GREP
AC_PROG_SED

# Checks for libraries.

AC_CHECK_LIB([sqlite3],
             [sqlite3_open],
             [],
             AC_MSG_ERROR([you need sqlite3]))

dnl  check for taglib
dnl  looking for taglib-config instead of -ltag, because of
dnl  configuration prog
AC_ARG_VAR(TAGLIBCONF, [holds wether or not taglib-config exists])
AC_CHECK_PROG(TAGLIBCONF, taglib-config, "yes", "no")

if test "$TAGLIBCONF" = "no"; then
	AC_MSG_ERROR([you need taglib-config])
fi

AC_CHECK_LIB([sqlite3],
             [sqlite3_open],
             [],
             AC_MSG_ERROR([you need sqlite3]))

# Checks for header files.
AC_HEADER_DIRENT
AC_CHECK_HEADERS([sys/statvfs.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_HEADER_STDC
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_STAT
AC_CHECK_FUNCS([memset mkdir strerror])

CPPFLAGS+=`taglib-config --cflags`
LDFLAGS+=`taglib-config --libs | $SED s,\ ,\\\n,g | $GREP '^-L'`
LIBS+=`taglib-config --libs | $SED s,\ ,\\\n,g | $GREP '^-l'`

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 data/setup.sh])
AC_OUTPUT
