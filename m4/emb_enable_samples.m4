# SYNOPSIS
#
#   EMB_ENABLE_SAMPLES
#
# DESCRIPTION
#
#   Adds '--enable-samples' argument.
#
#   Sets HAVE_GLFW automake variable if libglfw is present. Also sets
#   GLFW_LIBS variable.
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

AC_DEFUN([EMB_ENABLE_SAMPLES],[
  AC_ARG_ENABLE([samples], 
    [AS_HELP_STRING([--enable-samples], [build samples [default=no]])])

  AS_IF([test "x$enable_samples" = "xyes"], [
    AC_CHECK_HEADER([GLFW/glfw3.h], [
      AC_CHECK_LIB([glfw], [glfwInit], [have_glfw=yes], [have_glfw=no])
    ], [
      have_glfw=no
    ])
  ], [
    have_glfw=no
  ])
  AS_IF([test "x$have_glfw" != "xyes"], [
    AS_IF([test "x$enable_samples" = "xyes"], [
      AC_MSG_ERROR([You need GLFW installed to build samples])
    ])
  ], [
    AC_SUBST([GLFW_LIBS], ["-lglfw"])
  ])
  AM_CONDITIONAL([HAVE_GLFW], [test "x$have_glfw" != "xno"])
])
