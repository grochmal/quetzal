#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([quetzal], [0.1], [michal dot jan dot grochmal at gmail dot com])
# The -Wall is for automake not for the compiler
AM_INIT_AUTOMAKE([-Wall])
AM_SILENT_RULES([no])
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([libquetzal/points.c])
AC_CONFIG_MACRO_DIRS([m4])
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
AC_DEFINE([POINT_CACHE], [512], [points to allocate with every malloc])
AC_DEFINE([DIM_CACHE], [256], [dimensions to allocate with every malloc])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h inttypes.h stdint.h math.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT64_T
AC_TYPE_UINT32_T
if test "xyes" = "x${ac_cv_c_uint64_t}"; then
  AC_SUBST([POINT_OFFSET_TYPE], [uint64_t])
else
# HAVE_UINT32_T ensures that uint32_t is defined unless the machine itself do
# not support such a type.  If your machine do not support 32 bit integers it
# is time to change it, we are not in the 1970s anymore.
  AC_SUBST([POINT_OFFSET_TYPE], [uint32_t])
fi

AC_TYPE_LONG_DOUBLE
if test "xyes" = "x${ac_cv_type_long_double}"; then
  AC_SUBST([DIM_PRECISION], [long double])
else
  AC_SUBST([DIM_PRECISION], [double])
fi

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 include/quetzal.h])
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

