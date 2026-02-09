#pragma once
#include "Renderer.h"
#include "Display.h"
#include "scenes/scene.h"
#include "Cube.h"

#include "EulerCamera.h"

#include "Texture.h"

namespace scene
{
	struct CubeModel
	{
		glm::vec3 translation = { 0.0f, .0f, .0f };
		glm::vec3 rotation = { 0.0f, .0f, .0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	};
	struct EnabledLights
	{
		bool directional = false;
		bool spot = false;
	};
	struct Material
	{
		Texture diffuse;
		Texture specular;
		float shininess;
	};
	struct PointLight
	{
		Cube cube = { glm::vec3(.0f, .0f, .0f) };
		glm::vec3 ambient = { .1, .1, .1 };
		glm::vec3 diffuse = { .5, .5, .5 };
		glm::vec3 specular = { 1, 1, 1 };
		glm::vec3 attenuation = { 1.0f, 0.09f, 0.032f };
		glm::vec3 translation = cube.GetPosition();
		glm::vec3 rotation = glm::vec3{ .0f };
		glm::vec3 scale = glm::vec3(.2f);
		bool enabled = true;
	};
	struct DirectionalLight
	{
		glm::vec3 direction = { -0.2f, -1.0f, -0.3f };
		glm::vec3 ambient = { .1, .1, .1 };
		glm::vec3 diffuse = { .5, .5, .5 };
		glm::vec3 specular = { 1, 1, 1 };
	};
	struct SpotLight
	{
		glm::vec3 position;
		glm::vec3 direction;
		float innerCutOff = 12.5;
		float outerCutOff = 17.5;
		glm::vec3 ambient = { .1, .1, .1 };
		glm::vec3 diffuse = { .5, .5, .5 };
		glm::vec3 specular = { 1, 1, 1 };
		glm::vec3 attenuation = { 1.0f, 0.09f, 0.032f };
	};
	class LightningScene : public Scene
	{
	public:
		LightningScene(Display* display);
		~LightningScene() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		inline const EulerCamera& GetCamera() const
		{
			return  m_camera;
		}
	private:
		void ProcessCameraInput(float deltaTime);

	private:
		std::string m_name;
		Shader cubeShader;
		Shader lightShader;
		Cube cube1;
		Cube directionalLightCube;
		Material material;
		PointLight pointLights[4];
		DirectionalLight directionalLight;
		SpotLight spotLight;
		EnabledLights enabled;
		glm::mat4 projection = (1.0f);
		Renderer renderer;
		glm::vec3 cubePositions[9] = {
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
		CubeModel model;
		EulerCamera m_camera;
		Display* m_display;
	};
}
