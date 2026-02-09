#include <GL/glew.h>
#include "ClearColorScene.h"

#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

namespace scene
{
	ClearColorScene::ClearColorScene(Display* display) : m_color(.3, .2, .8, 1.0f), m_name("ClearColorScene"), m_display(display)
	{
	}

	ClearColorScene::~ClearColorScene()
	{
		glClearColor(.0f, .0f, .0f, 1.0f);
	}

	void ClearColorScene::OnUpdate(float deltaTime)
	{
	}

	void ClearColorScene::OnRender()
	{
	}
	void ClearColorScene::OnImGuiRender()
	{
		glClearColor(m_color.r, m_color.g, m_color.b, m_color.a);
		ImGui::ColorEdit4("Background Color", glm::value_ptr(m_color));
	}
}