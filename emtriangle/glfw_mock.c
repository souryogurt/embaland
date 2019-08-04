/**
 * @file
 * Mock of libglfw3 library
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <GLFW/glfw3.h>

#include <cgreen/mocks.h>

GLFWAPI int glfwInit(void)
{
	return (int)mock();
}

GLFWAPI void glfwTerminate(void)
{
	mock();
}
