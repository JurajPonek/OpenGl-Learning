#include "KeyBoardInput.h"

bool KeyBoardInput::IsKeyPressed(GLFWwindow* window, int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}