/**
 * @file
 * emtriangle application
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "emtriangle.h"

#include <GLFW/glfw3.h>
#include <embaland/embaland.h>

static emb_instance emb;

int application_main(int argc, char **argv)
{
	int retval = EXIT_SUCCESS;
	if (!glfwInit()) {
		retval = EXIT_FAILURE;
		goto exit;
	}
	if (emb_create(&emb) != EMB_SUCCESS) {
		retval = EXIT_FAILURE;
		goto glfw_terminate;
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWwindow *win = glfwCreateWindow(960, 540, "Triangle", NULL, NULL);
	if (win == NULL) {
		retval = EXIT_FAILURE;
		goto emb_destroy;
	}
	while (!glfwWindowShouldClose(win)) {
		glfwPollEvents();
	}

	glfwDestroyWindow(win);
emb_destroy:
	emb_destroy(emb);
glfw_terminate:
	glfwTerminate();
exit:
	return retval;
}
