#include "scene.h"

scene::MenuScene::MenuScene(Scene*& currentScene, Display* display)
	:m_currentScene(currentScene), m_display(display)
{
}

void scene::MenuScene::OnImGuiRender()
{
	if (ImGui::CollapsingHeader("Scene Selection")) {
		for (const auto& scene : m_scenes)
		{
			if (ImGui::Button(scene.first.c_str()))
			{
				m_currentScene = scene.second(m_display);
			}
		}
	}
}