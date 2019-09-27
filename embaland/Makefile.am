lib_LTLIBRARIES += libembaland.la
libembaland_la_SOURCES = embaland/ref.h\
			 embaland/ref.c\
			 embaland/compiler.h\
			 embaland/list.h\
			 embaland/list.c\
			 embaland/object.h\
			 embaland/object.c\
			 embaland/instance.h\
			 embaland/instance.c\
			 embaland/viewport.h\
			 embaland/viewport.c\
			 embaland/buffer.h\
			 embaland/buffer.c\
			 embaland/buffer_view.h\
			 embaland/buffer_view.c\
			 embaland/accessor.h\
			 embaland/accessor.c

nobase_include_HEADERS = embaland/embaland.h\
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

TESTS += embaland/ref_test
check_PROGRAMS += embaland/ref_test
embaland_ref_test_SOURCES = embaland/ref_test.c
embaland_ref_test_LDFLAGS = -static
embaland_ref_test_LDADD = libembaland.la $(CGREEN_LIBS)

TESTS += embaland/list_test
check_PROGRAMS += embaland/list_test
embaland_list_test_SOURCES = embaland/list_test.c
embaland_list_test_LDFLAGS = -static
embaland_list_test_LDADD = libembaland.la $(CGREEN_LIBS)

TESTS += embaland/object_test
check_PROGRAMS += embaland/object_test
embaland_object_test_SOURCES = embaland/object_test.c
embaland_object_test_LDFLAGS = -static
embaland_object_test_LDADD = libembaland.la $(CGREEN_LIBS)
endif
