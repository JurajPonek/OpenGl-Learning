#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <cmath>
#include <filesystem>
#include <format>
#include "Cube.h"
#include "Display.h"
#include "EulerCamera.h"
#include "QuaternionCamera.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "graphics/Renderer.h"
#include "graphics/Texture.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Model.h"
#include "scenes/LightningScene.h"
#include "scenes/scene.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "scenes/ClearColorScene.h"
#include "scenes/ModelScene.h"

int main(void)
{
	using namespace scene;
	Display display(1600, 1000, "OpenGl");

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Renderer renderer;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(display.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	Scene* currentScene = nullptr;
	MenuScene* menu = new MenuScene(currentScene, &display);
	ModelScene* modelScene = new ModelScene(&display);

	currentScene = modelScene;
	menu->AddScene<ClearColorScene>("Clear Color Scene");
	menu->AddScene<LightningScene>("Lightning Scene");
	menu->AddScene<ModelScene>("Model Scene");

	while (!display.ShouldWindowClose())
	{
		renderer.Clear();
		//glClearColor(0.902f, 0.902f, 0.980f, 1.0f);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		display.UpdateInputMode();
		float now = glfwGetTime();
		display.ProcessInput();

		if (currentScene)
		{
			currentScene->OnUpdate(deltaTime);
			currentScene->OnRender();
			if (display.renderUI())
			{
				ImGui::Begin("Control panel");

				if (currentScene != menu && ImGui::Button("Return"))
				{
					delete currentScene;
					currentScene = menu;
				}

				currentScene->OnImGuiRender();

				ImGui::End();
			}
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		display.SwapBuffers();
		glfwPollEvents();

		deltaTime = now - lastFrame;
		lastFrame = now;
	}
	delete currentScene;
	if (currentScene != menu)
	{
		delete menu;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;
}