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

GLFWAPI void glfwWindowHint(int hint, int value)
{
	mock(hint, value);
}

GLFWAPI GLFWwindow *glfwCreateWindow(int width, int height, const char *title,
				     GLFWmonitor *monitor, GLFWwindow *share)
{
	return (GLFWwindow *)mock(width, height, title, monitor, share);
}

GLFWAPI int glfwWindowShouldClose(GLFWwindow *window)
{
	return (int)mock(window);
}

GLFWAPI void glfwPollEvents(void)
{
	mock();
}

GLFWAPI void glfwDestroyWindow(GLFWwindow *window)
{
	mock(window);
}

GLFWAPI const char **glfwGetRequiredInstanceExtensions(uint32_t *count)
{
	return (const char **)mock(count);
}
