#include "LightningScene.h"
#include <format>
#include "KeyBoardInput.h"
#include "MouseInput.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

namespace scene
{
	LightningScene::LightningScene(Display* display)
		: m_name("LightningScene"), cubeShader("res/shaders/Cube.shader"), lightShader("res/shaders/Light.shader"),
		cube1(glm::vec3(0.0f, 0.0f, 0.0f)), directionalLightCube(glm::vec3(.0f, 5.0f, -5.0f)), m_camera(glm::vec3(0.0f, 0.0f, 3.0f), 45.0f, static_cast<float>(display->getWidth()) / display->getHeight(), .1f, 100.0f),
		m_display(display)
	{
		glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
		};

		glm::mat4 projection(1.0f);
		directionalLightCube.Scale(glm::vec3(.2f));
		material = Material{
			Texture("res/textures/container2.png"),
			Texture("res/textures/container2_specular.png"),
			32
		};

		directionalLight = DirectionalLight{};
		spotLight = SpotLight{};

		for (int i = 0; i < 4; i++)
		{
			pointLights[i].cube = Cube(pointLightPositions[i]);
			pointLights[i].translation = pointLightPositions[i];
		}
		enabled = EnabledLights{};
	}
	LightningScene::~LightningScene()
	{
	}

	void LightningScene::OnUpdate(float deltaTime)
	{
		ProcessCameraInput(deltaTime);
	}
	void LightningScene::OnRender()
	{
		cube1.UpdateModelMatrix(model.translation, model.rotation, model.scale);

		projection = glm::perspective(glm::radians(m_camera.getFov()), m_camera.getAspectRatio(), m_camera.getNearPlane(), m_camera.getFarPlane());
		glm::mat4 vp = projection * m_camera.getViewMatrix();

		cubeShader.Bind();
		cubeShader.SetMatrix4f("vp", vp);
		cubeShader.SetMatrix4f("model", cube1.GetModelMatrix());

		glm::mat3 normalMatrix = glm::transpose(glm::inverse(cube1.GetModelMatrix()));
		cubeShader.SetMatrix3f("normalMatrix", normalMatrix);

		cubeShader.SetUniform3v("viewPos", m_camera.getPosition());
		material.diffuse.Bind(0);
		cubeShader.SetInt("material.diffuse", 0);
		material.specular.Bind(1);
		cubeShader.SetInt("material.specular", 1);

		cubeShader.SetFloat("material.shininess", material.shininess);
		for (int i = 0; i < 4; i++)
		{
			pointLights[i].cube.UpdateModelMatrix(pointLights[i].translation, pointLights[i].rotation, pointLights[i].scale);
			cubeShader.SetUniform3v(std::format("pointLights[{}].position", i), pointLights[i].translation);
			cubeShader.SetUniform3v(std::format("pointLights[{}].ambient", i), pointLights[i].ambient);
			cubeShader.SetUniform3v(std::format("pointLights[{}].diffuse", i), pointLights[i].diffuse);
			cubeShader.SetUniform3v(std::format("pointLights[{}].specular", i), pointLights[i].specular);
			cubeShader.SetUniform3v(std::format("pointLights[{}].attenuation", i), pointLights[i].attenuation);
			cubeShader.SetBool(std::format("pointLights[{}].enabled", i), pointLights[i].enabled);
		}

		cubeShader.SetUniform3v("directionalLight.ambient", directionalLight.ambient);
		cubeShader.SetUniform3v("directionalLight.diffuse", directionalLight.diffuse);
		cubeShader.SetUniform3v("directionalLight.specular", directionalLight.specular);
		cubeShader.SetUniform3v("directionalLight.direction", directionalLight.direction);

		cubeShader.SetBool("enabled.directional", enabled.directional);
		cubeShader.SetBool("enabled.spot", enabled.spot);

		cubeShader.SetUniform3v("spotLight.position", m_camera.getPosition());
		cubeShader.SetUniform3v("spotLight.direction", m_camera.GetFront());
		cubeShader.SetFloat("spotLight.innerCutOff", glm::cos(glm::radians(spotLight.innerCutOff)));
		cubeShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotLight.outerCutOff)));
		cubeShader.SetUniform3v("spotLight.ambient", spotLight.ambient);
		cubeShader.SetUniform3v("spotLight.diffuse", spotLight.diffuse);
		cubeShader.SetUniform3v("spotLight.specular", spotLight.specular);
		cubeShader.SetUniform3v("spotLight.attenuation", spotLight.attenuation);

		for (const auto& pos : cubePositions)
		{
			Cube cube(pos);
			cubeShader.SetMatrix4f("model", cube.GetModelMatrix());
			cube.Draw(renderer, cubeShader);
		}
		cubeShader.SetMatrix4f("model", cube1.GetModelMatrix());
		cube1.Draw(renderer, cubeShader);

		lightShader.Bind();
		lightShader.SetMatrix4f("vp", vp);

		for (int i = 0; i < 4; i++)
		{
			lightShader.SetMatrix4f("model", pointLights[i].cube.GetModelMatrix());
			lightShader.SetUniform3v("u_lightColor", pointLights[i].diffuse);
			pointLights[i].cube.Draw(renderer, lightShader);
		}

		lightShader.Bind();
		lightShader.SetMatrix4f("vp", vp);
		lightShader.SetMatrix4f("model", directionalLightCube.GetModelMatrix());
		lightShader.SetUniform3v("u_lightColor", glm::vec3(1, 1, 1));
		directionalLightCube.Draw(renderer, lightShader);

		material.diffuse.UnBind();
		material.specular.UnBind();
	}
	void LightningScene::OnImGuiRender()
	{
		static int current_idx = 0;
		if (ImGui::CollapsingHeader("Cube"))
		{
			ImGui::PushID("Cube");
			ImGui::DragFloat3("Translation", glm::value_ptr(model.translation), 0.05f, -10.0f, 10.0f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(model.rotation), 1.0f, -360.0f, 360.0f);
			ImGui::DragFloat3("Scale", glm::value_ptr(model.scale), 0.05f, -10.0f, 10.0f);
			ImGui::Spacing();
			ImGui::DragFloat("Shininess", &material.shininess, 2, 2, 256);
			ImGui::PopID();
		}
		ImGui::Spacing();
		ImGui::Spacing();
		std::string comboPreview = "PointLight " + std::to_string(current_idx);
		if (ImGui::BeginCombo("Select Point Light", comboPreview.c_str()))
		{
			for (int i = 0; i < 4; i++)
			{
				const bool isSelected = (current_idx == i);
				std::string label = "PointLight " + std::to_string(i);

				if (ImGui::Selectable(label.c_str(), isSelected))
				{
					current_idx = i;
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Separator();
		auto& selectedLight = pointLights[current_idx];
		ImGui::PushID(current_idx);
		ImGui::Text("Point Light %d", current_idx);
		ImGui::Checkbox("Enabled", &selectedLight.enabled);
		ImGui::DragFloat3("Translation", glm::value_ptr(selectedLight.translation), 0.05f, -10.0f, 10.0f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(selectedLight.rotation), 1.0f, -360.0f, 360.0f);
		ImGui::DragFloat3("Scale", glm::value_ptr(selectedLight.scale), 0.05f, 0.01f, 10.0f);
		ImGui::Spacing();
		ImGui::ColorEdit3("Ambient", glm::value_ptr(selectedLight.ambient));
		ImGui::ColorEdit3("Diffuse", glm::value_ptr(selectedLight.diffuse));
		ImGui::ColorEdit3("Specular", glm::value_ptr(selectedLight.specular));
		ImGui::Spacing();
		ImGui::DragFloat3("Attenuation", glm::value_ptr(selectedLight.attenuation), 0.001f, 0.0f, 1.0f);
		ImGui::PopID();
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("DirectionalLight"))
		{
			ImGui::PushID("DirectionalLight");
			ImGui::Checkbox("DirectionalLight", &enabled.directional);
			ImGui::DragFloat3("Ambient", glm::value_ptr(directionalLight.ambient), 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat3("Diffuse", glm::value_ptr(directionalLight.diffuse), 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat3("Specular", glm::value_ptr(directionalLight.specular), 0.05f, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::DragFloat3("Direction", glm::value_ptr(directionalLight.direction), 0.05f, -1.0f, 1.0f);
			ImGui::PopID();
		}
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::CollapsingHeader("SpotLight"))
		{
			ImGui::PushID("SpotLight");
			ImGui::Checkbox("SpotLight", &enabled.spot);
			ImGui::DragFloat3("Ambient", glm::value_ptr(spotLight.ambient), 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat3("Diffuse", glm::value_ptr(spotLight.diffuse), 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat3("Specular", glm::value_ptr(spotLight.specular), 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat3("Attenuation", glm::value_ptr(spotLight.attenuation), 0.05f, 0.0f, 1.0f);
			ImGui::Spacing();
			ImGui::DragFloat("InnerCutOff", &spotLight.innerCutOff, 0.05f, 0.0f, 45.0f);
			ImGui::DragFloat("OuterCutOff", &spotLight.outerCutOff, 0.05f, 0.0f, 45.0f);
			ImGui::PopID();
		}
	}
	void LightningScene::ProcessCameraInput(float deltaTime)
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