#include <iostream>
#include <stdint.h>
#include <GL/glew.h>
#include <glfw3.h>

#include "Engine.h"
#include "Core/ResourceManagerCore/Pool/PoolBase.h"
#include "Core/ResourceManagerCore/Policy/MeshAllocationPolicy.h"
#include "Core/GameCore/GlobalProperties.h"

using namespace Game;

bool bMouseMove = false;

void get_window_pos(GLFWwindow* window)
{
	int32_t x, y;
	glfwGetWindowPos(window, &x, &y);

	GlobalProperties::GetInstance()->GetInputData().SetWindowPos(x, y);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	int32_t xPos = static_cast<int32_t>(xpos);
	int32_t yPos = static_cast<int32_t>(ypos);

	GlobalProperties::GetInstance()->GetInputData().SetMouseX(xPos);
	GlobalProperties::GetInstance()->GetInputData().SetMouseY(yPos);

	bMouseMove = true;
}

void get_screen_rezolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	GlobalProperties::GetInstance()->GetInputData().SetScreenWidth(mode->width);
	GlobalProperties::GetInstance()->GetInputData().SetScreenHeight(mode->height);
}

void get_window_size(GLFWwindow* window)
{
	int32_t width, height;
	glfwGetWindowSize(window, &width, &height);

	GlobalProperties::GetInstance()->GetInputData().SetWindowWidth(width);
	GlobalProperties::GetInstance()->GetInputData().SetWindowHeight(height);
}

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

	get_screen_rezolution();
	get_window_size(window);
	get_window_pos(window);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

		if (bMouseMove)
		{
			engine.MouseMove();
			bMouseMove = false;
		}
	}

	glfwTerminate();

	std::cin.get();

	return 0;
}