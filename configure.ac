AC_PREREQ([2.69])
AC_INIT([embaland],[0.1.0],[Egor Artemov <egor.artemov@gmail.com>],[embaland],[https://github.com/souryogurt/embaland])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([emtriangle/emtriangle.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror 1.11.6 foreign subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AM_MAINTAINER_MODE([enable])
# Checks for programs.
AC_PROG_CC
AM_PROG_AR
LT_INIT([win32-dll])

AX_SPLIT_VERSION
AC_DEFINE_UNQUOTED([VERSION_MAJOR], [${AX_MAJOR_VERSION}], [Major version number of package])
AC_DEFINE_UNQUOTED([VERSION_MINOR], [${AX_MINOR_VERSION}], [Minor version number of package])
AC_DEFINE_UNQUOTED([VERSION_PATCH], [${AX_POINT_VERSION}], [Patch version number of package])

dnl Required vulkan dependency
AC_CHECK_HEADER([vulkan/vulkan.h], [], [
  AC_MSG_ERROR([vulkan/vulkan.h not found])
])
AC_CHECK_LIB([vulkan], [vkCreateInstance], [
  AC_SUBST([VULKAN_LIBS], ["-lvulkan"])
], [
  AC_MSG_ERROR([libvulkan not found])
])

EMB_ENABLE_SAMPLES
EMB_ENABLE_UNIT_TESTS

AM_CONDITIONAL([EMB_BUILD_DLL], [test "x$enable_shared" = xyes])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
