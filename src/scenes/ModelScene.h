#pragma once
#include "Model.h"
#include "Renderer.h"
#include "scene.h"
#include "Sphere.h"
#include "Texture.h"

namespace scene
{
	struct SphereModel
	{
		glm::vec3 translation = { 0.0f, .0f, .0f };
		glm::vec3 rotation = { 0.0f, .0f, .0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
		Texture texture;
	};
	class ModelScene : public Scene
	{
	public:
		ModelScene(Display* display);
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		void ProcessCameraInput(float deltaTime);
	private:
		Display* m_display;
		EulerCamera m_camera;
		Model m_model;
		Shader m_shader;
		glm::mat4 m_projection;
		glm::mat4 m_modelMatrix;
		Renderer m_renderer;
		Sphere m_sphere;
		Shader m_SphereShader;
		SphereModel m_sphereModel;
	};
}
