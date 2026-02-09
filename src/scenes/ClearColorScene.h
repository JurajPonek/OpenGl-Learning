#pragma once
#include <string>

#include "scene.h"
#include "glm/vec4.hpp"

namespace scene
{
	class ClearColorScene : public Scene
	{
	public:
		ClearColorScene(Display* display);
		~ClearColorScene() override;
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		glm::vec4 m_color;
		std::string m_name;
		Display* m_display;
	};
}
