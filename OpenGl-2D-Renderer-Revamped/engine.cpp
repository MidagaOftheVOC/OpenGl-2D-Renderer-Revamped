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
	m_Renderer.SetResService(&m_ResourceService);
	m_Renderer.Init();

	m_UIManager.Init(&m_ResourceService, &m_InputController);
	m_UIManager.SetZSpaceDistribution(1, 2);

	m_InputController.SetTrackedKeystatesBitmask(
		InputController::c_ArrowTrackBit |
		InputController::c_LetterTrackBit |
		InputController::c_SpecialTrackBit
	);

	m_ResourceService.StartLoadingProcess();

	m_TextFactory = TextFactory(&GetResourceService());
	m_PaneFactory = PaneFactory(&GetResourceService());
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

	return true;
}

void Engine2D::QueueFreebatchesToRenderer(
	GameLoopReturnType& gameLoopRetVal
) {
	size_t CommCount = gameLoopRetVal.RenderCommands.size();

	float nearZ = GetUIManager().GetFurtherZBound();
	float farZ = GetRenderer().GetFarZCoord();

	float range = farZ - nearZ;
	float layerStep = range / static_cast<float>(CommCount);
	
	std::ranges::stable_sort(gameLoopRetVal.RenderCommands, [](const RenderCommand& a, const RenderCommand& b) {
		return a.IssuedZLayer < b.IssuedZLayer;
	});

	for (size_t i = 0; i < gameLoopRetVal.RenderCommands.size(); i++) {
		const auto& rCommand = gameLoopRetVal.RenderCommands.at(i);
		float calculatedZcoord = nearZ + i * layerStep;
		switch (rCommand.StoredValueType) {
			case RenderCommand::Type::FBatchDC: {
				GetRenderer().Draw(rCommand.Batch, 0, 0, calculatedZcoord, nullptr);
				break;
			}
			case RenderCommand::Type::TextDC: {
				GetRenderer().Draw(rCommand.Text, 90, 90, calculatedZcoord, nullptr);
				break;
			}
		}
	}
}

void Engine2D::ExecuteFrame(
	std::function<void(float, GameInput, GameLoopReturnType&)> gameLoop
) {
	GameLoopReturnType StoredRenderCommands;
	using Clock = std::chrono::steady_clock;
	auto start = Clock::now();

	glfwPollEvents();

	m_InputController.CaptureKeystates();

	//	This shuold probably be moved to InputController and add it as dependancy for UI manager
	//CapturedStates uiCapturedStates = m_UIManager.InterpretInput(m_InputController.ExposeGameInput());
	//
	//if (uiCapturedStates.capturedKeyboard)	m_InputController.ExposeGameInput().SetKeyboardCapturedFlag();
	//if (uiCapturedStates.capturedMouse)		m_InputController.ExposeGameInput().SetMouseCapturedFlag();

	gameLoop(m_ElapsedTimeSeconds, m_InputController.ExposeGameInput(), StoredRenderCommands);

	//	Also, we must allow for other types to be queued for drawing.
	QueueFreebatchesToRenderer(StoredRenderCommands);

	//	TODO:	decode the Z value
	DEBUG_LOG("UIBatch rendering is commented out.");
	//m_Renderer.Draw(GetResourceService().GetUIBatch(), 0, 0, 1, nullptr);

	m_Renderer.ExecuteDraws();

	auto end = Clock::now();

	m_ElapsedTimeSeconds = std::chrono::duration<float>(end - start).count();
}

/*
plan:

UI needs further testing once Texts are done.

Text rework:
Texts will stop being their own specialised batches. They'll continue to own the UTF string inside them, as well as a
TextOptions member variable, but won't hold their own OpenGL objects. 

Fonts must have:
pointer/ref to the spritesheet which coresponds to them
a bunch of data regarding the glyphs like Advance array




Low prio:
We should probably check if the Camera type var works as it should inside the renderer and how moving it affects the screen.
Should probably make the input for the lambda be just a non-const Engine* and pass this. The user can set their own names for ptrs to subsystems
*/

/*
MAKE SEPARATE FACTORIES FOR EACH TYPE THAT NEEDS FACTORIES
(and make them depend on res service)
*/
