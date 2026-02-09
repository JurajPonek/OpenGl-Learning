#include "ModelScene.h"

#include "KeyBoardInput.h"
#include "MouseInput.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace scene
{
	ModelScene::ModelScene(Display* display)
		:m_display(display), m_camera(glm::vec3(0.0f, 0.0f, 3.0f), 45.0f, static_cast<float>(display->getWidth()) / display->getHeight(), .1f, 100.0f),
		m_model(Model("res/models/backpack/backpack.obj")), m_shader("res/shaders/Model.shader"), m_projection(1.0f), m_modelMatrix(1.0f),
		m_sphere(36, 18), m_SphereShader("res/shaders/Sphere.shader"), m_sphereModel()

	{
		m_sphereModel.translation = { 0,0,-5.0f };
		m_sphereModel.texture = Texture("res/textures/wall.jpg");
	}

	void ModelScene::OnUpdate(float deltaTime)
	{
		ProcessCameraInput(deltaTime);
		m_sphere.UpdateModelMatrix(m_sphereModel.translation, m_sphereModel.rotation, m_sphereModel.scale);
	}

	void ModelScene::OnRender()
	{
		m_projection = glm::perspective(glm::radians(m_camera.getFov()), m_camera.getAspectRatio(), m_camera.getNearPlane(), m_camera.getFarPlane());
		glm::mat4 vp = m_projection * m_camera.getViewMatrix();
		m_shader.Bind();
		m_shader.SetMatrix4f("vp", vp);
		m_shader.SetMatrix4f("model", m_modelMatrix);
		m_model.Draw(m_shader);

		m_SphereShader.Bind();
		m_SphereShader.SetMatrix4f("vp", vp);
		m_SphereShader.SetMatrix4f("model", m_sphere.GetModelMatrix());
		m_sphereModel.texture.Bind(0);
		m_SphereShader.SetInt("tex", 0);

		m_sphere.Draw(m_renderer, m_SphereShader);
	}

	void ModelScene::OnImGuiRender()
	{
		ImGui::DragFloat3("Translation", glm::value_ptr(m_sphereModel.translation), 0.05f, -10.0f, 10.0f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(m_sphereModel.rotation), 1.0f, -360.0f, 360.0f);
		ImGui::DragFloat3("Scale", glm::value_ptr(m_sphereModel.scale), 0.05f, 0.01f, 10.0f);
	}
	void ModelScene::ProcessCameraInput(float deltaTime)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (!io.WantCaptureMouse) {
			m_camera.processMouseMovement(MouseInput::s_offsetX, MouseInput::s_offsetY);
			MouseInput::s_offsetX = 0.0f;
			MouseInput::s_offsetY = 0.0f;
			m_camera.processMouseScroll(MouseInput::s_scrollOffsetY);
			MouseInput::s_scrollOffsetY = 0.0f;
		}

		if (!io.WantCaptureKeyboard) {
			if (KeyBoardInput::IsKeyPressed(m_display->GetWindow(), GLFW_KEY_W))
			{
				m_camera.processKeyBoard(CameraDirection::FORWARD, deltaTime);
			}
			if (KeyBoardInput::IsKeyPressed(m_display->GetWindow(), GLFW_KEY_S))
			{
				m_camera.processKeyBoard(CameraDirection::BACKWARD, deltaTime);
			}
			if (KeyBoardInput::IsKeyPressed(m_display->GetWindow(), GLFW_KEY_D))
			{
				m_camera.processKeyBoard(CameraDirection::RIGHT, deltaTime);
			}
			if (KeyBoardInput::IsKeyPressed(m_display->GetWindow(), GLFW_KEY_A))
			{
				m_camera.processKeyBoard(CameraDirection::LEFT, deltaTime);
			}
		}
	}
}