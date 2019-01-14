#include <iostream>
#include <stdint.h>
#include <GL/glew.h>
#include <glfw3.h>

#include "Engine.h"
#include "Core/ResourceManagerCore/Pool/PoolBase.h"
#include "Core/ResourceManagerCore/Policy/MeshAllocationPolicy.h"

int32_t main(int32_t argc, char** argv)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "PHEngine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLenum initResult = glewInit();
	if (initResult != GLEW_OK)
	{
		std::cout << "Something is wrong" << std::endl;
		glfwTerminate();
	}

	Engine engine;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		engine.TickWindow();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();

	std::cin.get();

	return 0;
}