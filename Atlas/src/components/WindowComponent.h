#pragma once

#include "Component.h"
#include "../utilities/Log.h"

#include <string>
#include <GLFW/glfw3.h>

struct WindowSettings
{
	WindowSettings(int Width, int Height, const std::string& Title, bool Fullscreen, bool Closable) : width(Width), height(Height), title(Title), fullscreen(Fullscreen), closable(Closable) {}

	int width;
	int height;
	std::string title;
	bool fullscreen;
	bool closable;
};

class WindowComponent : public Component
{
public:
	WindowComponent(const std::string& tag, int width, int height, const std::string& title, bool fullscreen = false, bool closable = true);

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate() override;

private:
	GLFWwindow* m_Window;
	WindowSettings m_Settings;
};

