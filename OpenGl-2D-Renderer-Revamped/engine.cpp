#include "engine.h"

void Engine2D::PreInit() {
	if (!GLFWInitialisation()) {
		throw new std::exception("idk bro");
	}

	m_Renderer = Renderer2D(
		m_MainWindowContext,
		m_ScreenWidth,
		m_ScreenHeight,
		m_WindowTitle.c_str(),
		m_Fullscreen
	);

	m_InputController = InputController(m_MainWindowContext);

	m_ResourceService = ResourceService(m_MainWindowContext);

}

void Engine2D::Init() {
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

	m_Renderer.Init();
	m_ResourceService.StartLoadingProcess();

	m_TextFactory = TextFactory(&GetResourceService());
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
