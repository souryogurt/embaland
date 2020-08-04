lib_LTLIBRARIES += libembaland.la
libembaland_la_SOURCES = embaland/compiler.h\
			 embaland/instance.h\
			 embaland/instance.c\
			 embaland/viewport.h\
			 embaland/viewport.c
libembaland_la_LIBADD = $(VULKAN_LIBS)

nobase_include_HEADERS = embaland/instance.h\
			 embaland/viewport.h\
			 embaland/platform.h
if EMB_BUILD_DLL
libembaland_la_CPPFLAGS = -D_EMB_BUILD_DLL
endif

if HAVE_CGREEN
TESTS += embaland/instance_test
check_PROGRAMS += embaland/instance_test
embaland_instance_test_SOURCES = embaland/instance_test.c
embaland_instance_test_LDFLAGS = -static
embaland_instance_test_LDADD = libembaland.la $(CGREEN_LIBS)

TESTS += embaland/viewport_test
check_PROGRAMS += embaland/viewport_test
embaland_viewport_test_SOURCES = embaland/viewport_test.c
embaland_viewport_test_LDFLAGS = -static
embaland_viewport_test_LDADD = libembaland.la $(CGREEN_LIBS)
endif
