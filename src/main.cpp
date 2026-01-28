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
#include "Texture.h"
#include "EulerCamera.h"
#include "QuaternionCamera.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void)
{
	Display display(1600, 1000, "Test");

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	float vertices[] = {
		// ===== zadna stena (-Z) =====
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 1
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 3

		// ===== predna stena (+Z) =====
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 5
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 7

		// ===== lava stena (-X) =====
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 8
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 9
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //10
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //11

		// ===== prava stena (+X) =====
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //12
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //13
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //14
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //15

		 // ===== spodna stena (-Y) =====
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //16
		  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, //17
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //18
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //19

		 // ===== horna stena (+Y) =====
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //20
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //21
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //22
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  //23
	};

	unsigned int indices[] = {
		// zadna stena
	 0,  1,  2,
	 2,  3,  0,

	 // predna stena
	  4,  5,  6,
	  6,  7,  4,

	  // lava stena
	   8,  9, 10,
	  10, 11,  8,

	  // prava stena
	  12, 13, 14,
	  14, 15, 12,

	  // spodna stena
	  16, 17, 18,
	  18, 19, 16,

	  // horna stena
	  20, 21, 22,
	  22, 23, 20
	};
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	Renderer renderer;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(display.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Shader basic("res/shaders/Basic.shader");
	basic.Bind();
	Texture texture1("res/textures/container.jpg");
	basic.SetInt("Texture1", 0);

	Texture texture2("res/textures/face.png");
	basic.SetInt("Texture2", 1);

	VertexArray vao;
	VertexBuffer vbo(vertices, sizeof(vertices));
	VertexBufferLayout layout;
	IndexBuffer ib(indices, 36);
	layout.Push<float>(3, false);
	layout.Push<float>(2, false);
	vao.AddVertexBuffer(vbo, layout);
	vao.AddIndexBuffer(ib);
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	EulerCamera camera(glm::vec3(0.0f, 0.0f, 3.0f), 45.0f, static_cast<float>(display.getWidth()) / display.getHeight(), .1f, 100.0f);
	glm::vec3 translation(0.0f);
	float angleX = 0.0f;
	float angleY = 0.0f;
	float angleZ = 0.0f;
	glm::vec3 scale(1.0f);
	while (!display.ShouldWindowClose())
	{
		renderer.Clear();
		display.UpdateInputMode();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float now = glfwGetTime();
		display.ProcessInput();
		display.ProcessCameraInput(camera, deltaTime);

		texture1.Bind(0);
		texture2.Bind(1);

		const float r = 10.f;
		float x = sin(glfwGetTime()) * r;
		float z = cos(glfwGetTime()) * r;

		glm::mat4 projection(1.0f);
		glm::mat4 model(1.0f);
		model = glm::translate(model, translation);
		model = glm::rotate(model, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale);
		projection = glm::perspective(glm::radians(camera.getFov()), camera.getAspectRatio(), camera.getNearPlane(), camera.getFarPlane());
		basic.SetMatrix4f("view", camera.getViewMatrix());
		basic.SetMatrix4f("projection", projection);
		basic.SetMatrix4f("model", model);
		basic.Bind();
		renderer.draw(vao, ib, basic);
		if (display.renderUI())
		{
			ImGui::Begin("Translation");

			ImGui::DragFloat3("Translation", glm::value_ptr(translation), 0.05f, -10.0f, 10.0f);
			ImGui::DragFloat("Rotation X", &angleX, 1.0f, -360.0f, 360.0f);
			ImGui::DragFloat("Rotation Y", &angleY, 1.0f, -360.0f, 360.0f);
			ImGui::DragFloat("Rotation Z", &angleZ, 1.0f, -360.0f, 360.0f);
			ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.05f, -10.0f, 10.0f);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		display.SwapBuffers();
		glfwPollEvents();

		deltaTime = lastFrame - now;
		lastFrame = now;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}