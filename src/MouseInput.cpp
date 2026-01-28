#include "MouseInput.h"

float MouseInput::s_offsetX = 0.0f;
float MouseInput::s_offsetY = 0.0f;
float MouseInput::s_scrollOffsetY = 0.0f;

void MouseInput::MouseCallBack(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouseMovement = true;
	if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
		firstMouseMovement = true;
		return;
	}

	static double lastX, lastY;

	if (firstMouseMovement)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouseMovement = false;
	}

	s_offsetX += static_cast<float>(xpos - lastX);
	s_offsetY += static_cast<float>(lastY - ypos);

	lastX = xpos;
	lastY = ypos;
}

void MouseInput::ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	s_scrollOffsetY = static_cast<float>(yoffset);
}