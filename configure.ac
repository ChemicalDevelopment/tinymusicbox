
AC_PREREQ([2.55])
AC_INIT([tinymusicbox], [0.0.1], [cade@cade.site])

AM_INIT_AUTOMAKE([])

AC_CONFIG_SRCDIR([src/tinymusicbox.h])
AC_CONFIG_HEADERS([config.h])

CFLAGS="$CFLAGS -Ofast -ffast-math"

AC_LANG(C)
AC_PROG_CC

AC_CHECK_HEADERS([portaudio.h stdlib.h stdio.h stdbool.h string.h math.h sys/time.h sys/stat.h time.h unistd.h assert.h stdarg.h getopt.h ctype.h dlfcn.h])

AC_CHECK_LIB(m, pow)

AC_CHECK_SIZEOF([double])
AC_CHECK_SIZEOF([unsigned char])
AC_CHECK_SIZEOF([unsigned short])
AC_CHECK_SIZEOF([unsigned int])
AC_CHECK_SIZEOF([unsigned long])

# Checks for library functions.
#AC_FUNC_MALLOC
#AC_FUNC_REALLOC

# todo, add tests
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
