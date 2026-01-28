#pragma once
#include <utility>
#include <GLFW/glfw3.h>
class MouseInput
{
public:
	static void MouseCallBack(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

public:
	static float s_offsetX;
	static float s_offsetY;
	static float s_scrollOffsetY;
};
