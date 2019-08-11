lib_LTLIBRARIES += libembaland.la
libembaland_la_SOURCES = embaland/embaland.c\
			 embaland/ref.h\
			 embaland/ref.c\
			 embaland/compiler.h\
			 embaland/list.h\
			 embaland/list.c\
			 embaland/string.h\
			 embaland/string.c\
			 embaland/object.h\
			 embaland/object.c

nobase_include_HEADERS = embaland/embaland.h\
			 embaland/platform.h
if EMB_BUILD_DLL
libembaland_la_CPPFLAGS = -D_EMB_BUILD_DLL
endif

if HAVE_CGREEN
TESTS += embaland/embaland_test
check_PROGRAMS += embaland/embaland_test
embaland_embaland_test_SOURCES = embaland/embaland_test.c
embaland_embaland_test_LDFLAGS = -static
embaland_embaland_test_LDADD = libembaland.la $(CGREEN_LIBS)

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
