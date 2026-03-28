#pragma once

#include "subsystems/main_renderer.h"

class Engine2D {

	Renderer2D m_Renderer;

	UIManager m_UIManager;

	InputController m_InputController;

private:

	GLFWwindow* m_MainWindowContext = nullptr;

	int m_ScreenWidth = -1;
	int m_ScreenHeight = -1;
	std::string m_WindowTitle = nullptr;

	bool m_Fullscreen = false;

	bool m_IsRunning = false;

public:

	Engine2D() {}

	Engine2D(
		int screenWidth,
		int screenHeight,
		const char* windowTitle,
		bool fullscreen
	)
		:
		m_ScreenWidth(screenWidth),
		m_ScreenHeight(screenHeight),
		m_WindowTitle(windowTitle),
		m_Fullscreen(fullscreen)
	{
		PreInit();
		Init();
	}

private:

	void PreInit();

	void Init();

	bool GLFWInitialisation();

};
