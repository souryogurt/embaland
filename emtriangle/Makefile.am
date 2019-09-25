bin_PROGRAMS += emtriangle/emtriangle
emtriangle_emtriangle_SOURCES = emtriangle/entry.c
emtriangle_emtriangle_LDADD = emtriangle/libemtriangle.la\
			      libembaland.la $(GLFW_LIBS) $(VULKAN_LIBS)

noinst_LTLIBRARIES += emtriangle/libemtriangle.la
emtriangle_libemtriangle_la_SOURCES = emtriangle/emtriangle.h\
				      emtriangle/emtriangle.c\
				      emtriangle/scene.h\
				      emtriangle/scene.c

if HAVE_CGREEN
TESTS += emtriangle/emtriangle_test
check_PROGRAMS += emtriangle/emtriangle_test
emtriangle_emtriangle_test_SOURCES = emtriangle/emtriangle_test.c\
				     emtriangle/glfw_mock.c\
				     emtriangle/vulkan_mock.c\
				     emtriangle/embaland_mock.c\
				     emtriangle/scene_mock.c
emtriangle_emtriangle_test_LDADD = emtriangle/libemtriangle.la $(CGREEN_LIBS)
endif
