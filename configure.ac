dnl ----------------------------------------------------------
dnl INITIALIZE CONFIGURE
dnl ----------------------------------------------------------
AC_INIT([novemberizing], [0.0.1], [novemberizing@gmail.com])
LT_INIT
dnl LT_LANG([C++])

dnl ----------------------------------------------------------
dnl CREATE CONFIGURATION FILES
dnl ----------------------------------------------------------
AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

dnl ----------------------------------------------------------
dnl CONFIGURATION HEADER FILES
dnl ----------------------------------------------------------
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])


dnl CFLAGS="-g -msse4.2"
CFLAGS=-g

dnl ----------------------------------------------------------
dnl AUTOMAKE
dnl ----------------------------------------------------------
AM_INIT_AUTOMAKE([subdir-objects])
dnl AC_PROG_RANLIB
dnl AC_PROG_CXX
AC_PROG_CC

dnl ----------------------------------------------------------
dnl OUTPUTING FILES
dnl ----------------------------------------------------------
AC_OUTPUT
