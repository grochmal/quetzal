#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([quetzal], [0.1], [michal.grochmal@gmail.com])
# The -Wall is for automake not for the compiler
AM_INIT_AUTOMAKE([-Wall])
AM_SILENT_RULES([no])
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/quetzalknn.c])
AC_CONFIG_HEADERS([config.h])
LT_PREREQ([2.2])
LT_INIT([dlopen])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
# AC_PROG_RANLIB
AC_PROG_INSTALL

# Check for command-line options

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT

echo \
"-----------------------------------------------------------

${PACKAGE_NAME} Version ${PACKAGE_VERSION}

Prefix: '${prefix}'
Compiler: '${CC} ${CFLAGS} ${CPPFLAGS}'
Libraries linked: '${LIBS}'

Package features:

Libraries used:

To build ${PACKAGE_NAME} type make @<:@target@:>@
where the optional <target> can be one of the following:
  all     - build all binaries
  install - install everything

------------------------------------------------------------"
