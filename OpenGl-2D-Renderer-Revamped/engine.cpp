#include "engine.h"

void Engine2D::PreInit() {
	if (!GLFWInitialisation()) {
		throw new std::exception("nigg");
	}

	m_Renderer = Renderer2D(
		m_MainWindowContext,
		m_ScreenWidth,
		m_ScreenHeight,
		m_WindowTitle.c_str(),
		m_Fullscreen
	);

	m_InputController = InputController(m_MainWindowContext);
}

void Engine2D::Init() {
	m_Renderer.SetUIManager(&m_UIManager);
	m_Renderer.SetInputController(&m_InputController);

	m_UIManager.SetRenderer(&m_Renderer);
	m_UIManager.SetInputController(&m_InputController);
	m_UIManager.SetDistributionBounds(1, 2);

	//	BATCH
	/*m_UIBatch.AddSheetToBatch(GetSpriteSheetByName(c_SpecialUISheetName));
	m_UIBatch.BufferUBOs();*/

	m_InputController.SetTrackedKeystatesBitmask(
		InputController::c_ArrowTrackBit |
		InputController::c_LetterTrackBit |
		InputController::c_SpecialTrackBit
	);


}


bool Engine2D::GLFWInitialisation() {
	//	Window and GLFW context initialisation
	if (!glfwInit()) {
		std::cout << "Window not opened.\n";
		return false;
	}

	m_MainWindowContext = glfwCreateWindow(
		m_ScreenWidth,
		m_ScreenHeight,
		m_WindowTitle.c_str(),
		NULL, NULL
	);

	if (!m_MainWindowContext) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_MainWindowContext);
	glfwSwapInterval(1);
	GLenum glewReturnCode = glewInit();
	if (glewReturnCode != GLEW_OK) {
		std::cout << glewGetErrorString(glewReturnCode);
		return false;
	}
}


void Renderer2D::LoadDefaultVariables() {
	m_DefaultFont = Font(
		GetSpriteSheetByName("test_font"),
		"cyrillic"
	);

	unsigned short off[] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, };
	m_DefaultFont.Init(
		U"абвгдежзийклмнопрстуфхцчшщъьюяѝ .,+-!?;:&><#/",
		off,
		50
	);

	m_DefaultTextOptions.m_Font = &m_DefaultFont;
}

Text Renderer2D::GenText(
	const char32_t* _string,
	TextOptions _textOptions
) const {

	if (_textOptions.m_Font == nullptr) {
		//	default;
		return Text(_string, m_DefaultTextOptions);
	}

	return Text(_string, _textOptions);
}


bool Renderer2D::IsRunning() const {
	return glfwWindowShouldClose(GetWinHandle());
}
