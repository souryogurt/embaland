# SYNOPSIS
#
#   EMB_ENABLE_UNIT_TESTS
#
# DESCRIPTION
#
#   Adds '--enable-unit-tests' argument.
#
#   Sets HAVE_CGREEN automake variable if libcgreen is present. Also sets
#   CGREEN_LIBS variable.
#
# LICENSE
#
#   Copyright (C) 2019 Egor Artemov <egor.artemov@gmail.com>
#
#   Everyone is permitted to copy and distribute verbatim or modified
#   copies of this license document, and changing it is allowed as long
#   as the name is changed.
#
#              DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
#     TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
#
#    0. You just DO WHAT THE FUCK YOU WANT TO.

#serial 1

AC_DEFUN([EMB_ENABLE_UNIT_TESTS],[
  AC_ARG_ENABLE([unit-tests],
    [AS_HELP_STRING([--disable-unit-tests], [disable unit testing])])
  AS_IF([test "x$enable_unit_tests" != "xno"], [
    AC_CHECK_HEADER([cgreen/cgreen.h], [
      AC_CHECK_LIB([cgreen], [run_test_suite], [have_cgreen=yes], [have_cgreen=no])
    ], [
      have_cgreen=no
    ])
  ], [
    have_cgreen=no
  ])
  AS_IF([test "x$have_cgreen" != "xyes"], [
    AS_IF([test "x$enable_unit_tests" = "xyes"], [
      AC_MSG_ERROR([You need cgreen installed to build unit tests])
    ])
  ], [
    AC_SUBST([CGREEN_LIBS], ["-lcgreen"])
  ])
  AM_CONDITIONAL([HAVE_CGREEN], [test "x$have_cgreen" != "xno"])
])
