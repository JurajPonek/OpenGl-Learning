#pragma once
#include <GLFW/glfw3.h>
class UI
{
	UI(GLFWwindow* window);
	void RenderUI() const;

	void CleanUp() const;
};
