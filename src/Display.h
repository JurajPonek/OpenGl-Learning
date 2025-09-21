#pragma once
#include <GLFW/glfw3.h>
#include <string>
class Display
{
private:
	int windowWidth;
	int windowHeight;
	GLFWwindow* window;

public:
	Display(int width, int height, const std::string& title);
	~Display();
	void ResizeWindow(int widht, int height);
	bool ShouldWindowClose() const;
	void SwapBuffers() const;
	inline GLFWwindow* GetWindow() const
	{
		return window;
	}
	void ProcessInput();

private:
	static void ResizeCallback(GLFWwindow* window, int width, int height);
	void InitializeDebugContext();
	void PrintSystemInfo();
};