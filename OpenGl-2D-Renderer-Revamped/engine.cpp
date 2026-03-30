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

void Engine2D::ExecuteFrame(
	std::function<GameLoopReturnType(float, GameInput)> gameLoop
) {
	using Clock = std::chrono::steady_clock;
	auto start = Clock::now();

	/*
	plan:

	Create a global Sprite type to consolidate sprite information
	Improve base batch to maintain a state and function for 'drawing' a sprite through it.
		-	Freebatch.Draw(Sprite, x, y). Where sprite's data is copied/appended depends on current drawn sprite count for that batch instance
		-	On render cycle reset this count. Note to this: it could probably be based on the instance count in the BaseBatch
		-	Note: we should probably remove the ability to copy entire arrays into the batch, this is precisely what we want to eliminate
	m_Renderer must be able to consume Freebatches
	
	!!! Work on program until Free batches confirmed working with .cfg files

	UIBatch inside UIManager and how to queues the render commands
	Rework the widgets
		-	Windows should hold an array of widgets
		-	Widgets need to maintain a list of widgets themselves to allow nesting
		-	Widget interface must be strict enough so the widget-click-detection function works recursively 
			-	This idea is very complicated, may not be that good


	Low prio:
	We should probably check if the Camera type var works as it should inside the renderer and how moving it affects the screen.

	*/

	/*
	IMPROVE BATCHES
	They should know when to update their buffers. They should have a fn DrawSprite(...) or something which takes SpriteInfo or something.
	Before rendering, update arrays if needed.

	Also make a struct to unify those 1-4 OUT_ arrays to make them easier to use.

	MAKE SEPARATE FACTORIES FOR EACH TYPE THAT NEEDS FACTORIES
	(and make them depend on res service)
	*/

	glfwPollEvents();

	m_InputController.CaptureKeystates();

	CapturedStates uiCapturedStates = m_UIManager.InterpretInput(m_InputController.ExposeGameInput());

	if (uiCapturedStates.capturedKeyboard)	m_InputController.ExposeGameInput().SetKeyboardCapturedFlag();
	if (uiCapturedStates.capturedMouse)		m_InputController.ExposeGameInput().SetMouseCapturedFlag();

	GameLoopReturnType gameLoopRenderData = gameLoop(m_ElapsedTimeSeconds, m_InputController.ExposeGameInput());

	//	Here m_Renderer consumes the UI batch from the manager and the game loop ret val

	m_Renderer.ExecuteDraws();

	auto end = Clock::now();

	m_ElapsedTimeSeconds = std::chrono::duration<float>(end - start).count();
}