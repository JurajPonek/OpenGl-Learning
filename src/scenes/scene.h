#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Display.h"
#include "imgui/imgui.h"

namespace scene
{
	class Scene
	{
	public:
		Scene() {};
		virtual ~Scene() {};
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class MenuScene : public Scene
	{
	public:
		MenuScene(Scene*& currentScene, Display* display);
		void OnImGuiRender() override;
		template<typename T>
		void AddScene(const std::string& name)
		{
			m_scenes.push_back(std::make_pair(name, [](Display* display) {return new T(display); }));
		}

	private:
		std::vector<std::pair<std::string, std::function<Scene* (Display*)>>> m_scenes;
		Scene*& m_currentScene;
		Display* m_display;
	};
}
