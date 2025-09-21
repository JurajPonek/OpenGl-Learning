#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Display.h"
#include "VertexArray.h"
#include "Renderer.h"

int main(void)
{
	Display display(800, 600, "Test");

	/*glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);*/

	float firstTriangel[] = {
		-0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
	};

	float secondTriangel[] = {
		0.0f, 0.0f,
		0.5f, 0.0f,
		0.0f, 0.5f,
	};
	unsigned int indices[] = {
		0,1,2,
		1,2,3
	};
	Shader basic("res/shaders/Basic.shader");

	Shader yellow("res/shaders/Yellow.shader");
	Renderer renderer;
	VertexArray va1, va2;
	VertexBuffer vbo1(&secondTriangel, sizeof(secondTriangel));
	VertexBuffer vbo2(&firstTriangel, sizeof(firstTriangel));
	VertexBufferLayout layout1;
	VertexBufferLayout layout2;
	layout1.Push<float>(2, false);
	va1.AddVertexBuffer(vbo1, layout1);
	va1.Unbind();

	va2.Bind();
	layout2.Push<float>(2, false);
	layout2.Push<float>(3, false);
	va2.AddVertexBuffer(vbo2, layout2);
	IndexBuffer indexBuffer(indices, 6);
	va2.AddIndexBuffer(indexBuffer);
	basic.Bind();
	basic.SetFloat("horizontalOffset", -0.5f);

	while (!display.ShouldWindowClose())
	{
		display.ProcessInput();

		renderer.draw(va2, indexBuffer, basic);

		display.SwapBuffers();
		glfwPollEvents();
	}

	return 0;
}