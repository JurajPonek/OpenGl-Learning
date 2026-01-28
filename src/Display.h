#pragma once
#include <GLFW/glfw3.h>
#include <string>

#include "EulerCamera.h"

class Display
{
private:
	int m_windowWidth;
	int m_windowHeight;
	GLFWwindow* m_window;

public:
	Display(int width, int height, const std::string& title);
	~Display();
	void ResizeWindow(int widht, int height);
	bool ShouldWindowClose() const;
	void SwapBuffers() const;
	inline GLFWwindow* GetWindow() const
	{
		return m_window;
	}
	inline int getWidth() const { return m_windowWidth; }
	inline int getHeight() const { return m_windowHeight; }
	void ProcessInput();
	void ProcessCameraInput(EulerCamera& camera, float deltaTime);

private:
	static void ResizeCallback(GLFWwindow* window, int width, int height);
	void InitializeDebugContext();
	void PrintSystemInfo();
};
