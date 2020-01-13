lib_LTLIBRARIES += libembaland.la
libembaland_la_SOURCES = embaland/compiler.h\
			 embaland/list.h\
			 embaland/list.c\
			 embaland/instance.h\
			 embaland/instance.c\
			 embaland/viewport.h\
			 embaland/viewport.c\
			 embaland/buffer.h\
			 embaland/buffer.c\
			 embaland/buffer_view.h\
			 embaland/buffer_view.c\
			 embaland/accessor.h\
			 embaland/accessor.c\
			 embaland/mesh.h\
			 embaland/mesh.c\
			 embaland/material.h\
			 embaland/material.c

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

TESTS += embaland/list_test
check_PROGRAMS += embaland/list_test
embaland_list_test_SOURCES = embaland/list_test.c
embaland_list_test_LDFLAGS = -static
embaland_list_test_LDADD = libembaland.la $(CGREEN_LIBS)

TESTS += embaland/buffer_test
check_PROGRAMS += embaland/buffer_test
embaland_buffer_test_SOURCES = embaland/buffer_test.c
embaland_buffer_test_LDFLAGS = -static
embaland_buffer_test_LDADD = libembaland.la $(CGREEN_LIBS)

TESTS += embaland/buffer_view_test
check_PROGRAMS += embaland/buffer_view_test
embaland_buffer_view_test_SOURCES = embaland/buffer_view_test.c
embaland_buffer_view_test_LDFLAGS = -static
embaland_buffer_view_test_LDADD = libembaland.la $(CGREEN_LIBS)

TESTS += embaland/accessor_test
check_PROGRAMS += embaland/accessor_test
embaland_accessor_test_SOURCES = embaland/accessor_test.c
embaland_accessor_test_LDFLAGS = -static
embaland_accessor_test_LDADD = libembaland.la $(CGREEN_LIBS)

TESTS += embaland/mesh_test
check_PROGRAMS += embaland/mesh_test
embaland_mesh_test_SOURCES = embaland/mesh_test.c
embaland_mesh_test_LDFLAGS = -static
embaland_mesh_test_LDADD = libembaland.la $(CGREEN_LIBS)

TESTS += embaland/material_test
check_PROGRAMS += embaland/material_test
embaland_material_test_SOURCES = embaland/material_test.c
embaland_material_test_LDFLAGS = -static
embaland_material_test_LDADD = libembaland.la $(CGREEN_LIBS)
endif
