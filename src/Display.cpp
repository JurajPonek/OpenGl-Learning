#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Display.h"
#include <iostream>
#include "KeyBoardInput.h"
#include "MouseInput.h"

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char* message, const void* userParam)
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;

	std::cout << "===============================\n";
	std::cout << "OpenGL Debug (" << id << "): " << message << std::endl;
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	}
	std::cout << std::endl;
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	}
	std::cout << std::endl;
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: HIGH"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: MEDIUM"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: LOW"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: NOTIFICATION"; break;
	}
	std::cout << std::endl;
	std::cout << "===============================\n";
}
Display::Display(int width, int height, const std::string& title) : m_windowWidth(width), m_windowHeight(height)
{
	if (!glfwInit())
	{
		std::cout << "Error while initializing GLFW";
		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		std::cout << "Error while creating m_window";
		return;
	}
	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error while initializing GLEW" << std::endl;
		return;
	}
	InitializeDebugContext();
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(m_window, ResizeCallback);
	glfwSetCursorPosCallback(m_window, MouseInput::MouseCallBack);
	glfwSetScrollCallback(m_window, MouseInput::ScrollCallBack);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	PrintSystemInfo();
}

Display::~Display()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Display::InitializeDebugContext()
{
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		std::cout << "\n✓ OpenGL Debug Context successfully created!" << std::endl;
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		std::cout << "Debug callbacks set up successfully!" << std::endl;
	}
	else
	{
		std::cout << "Debug context not available - using fallback error checking" << std::endl;
	}
}

void Display::PrintSystemInfo()
{
	std::cout << "\n=== OpenGL System Info ===" << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "==========================\n" << std::endl;
}

void Display::ResizeWindow(int width, int height)
{
	glfwSetWindowSize(m_window, width, height);
}

bool Display::ShouldWindowClose() const
{
	return glfwWindowShouldClose(m_window);
}

void Display::SwapBuffers() const
{
	glfwSwapBuffers(m_window);
}

void Display::ResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Display::ProcessInput()
{
	if (KeyBoardInput::IsKeyPressed(m_window, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(m_window, true);
	}
}
void Display::ProcessCameraInput(EulerCamera& camera, float deltaTime)
{
	if (KeyBoardInput::IsKeyPressed(m_window, GLFW_KEY_W))
	{
		camera.processKeyBoard(CameraDirection::FORWARD, deltaTime);
	}
	if (KeyBoardInput::IsKeyPressed(m_window, GLFW_KEY_S))
	{
		camera.processKeyBoard(CameraDirection::BACKWARD, deltaTime);
	}
	if (KeyBoardInput::IsKeyPressed(m_window, GLFW_KEY_D))
	{
		camera.processKeyBoard(CameraDirection::RIGHT, deltaTime);
	}
	if (KeyBoardInput::IsKeyPressed(m_window, GLFW_KEY_A))
	{
		camera.processKeyBoard(CameraDirection::LEFT, deltaTime);
	}

	camera.processMouseMovement(MouseInput::s_offsetX, MouseInput::s_offsetY);
	MouseInput::s_offsetX = 0.0f;
	MouseInput::s_offsetY = 0.0f;
	camera.processMouseScroll(MouseInput::s_scrollOffsetY);
	MouseInput::s_scrollOffsetY = 0.0f;
}