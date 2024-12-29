#include "WindowComponent.h"



WindowComponent::WindowComponent(const std::string& tag, int width, int height, const std::string& title, bool fullscreen, bool closable) : Component(tag), m_Window(nullptr), m_Settings(width, height, title, fullscreen, closable)
{
	if (!glfwInit())
		ATLAS_ERROR("Window component -> " + tag + " failed to init window!");
}

void WindowComponent::OnAttach()
{
	m_Window = glfwCreateWindow(m_Settings.width, m_Settings.height, m_Settings.title.c_str(), m_Settings.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (!m_Window)
	{
		ATLAS_ERROR("Window component -> " + Tag + " failed to create window!");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);
}

void WindowComponent::OnUpdate()
{
	if (!glfwWindowShouldClose(m_Window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(m_Window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	else
	{
		glfwTerminate();
	}
}

void WindowComponent::OnDetach()
{
	glfwTerminate();
}