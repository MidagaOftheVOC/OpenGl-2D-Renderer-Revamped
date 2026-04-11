#include "engine.h"
#include <ranges>

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
	m_UIManager.SetPaneFactory(&m_PaneFactory);
	m_UIManager.SetDistributionBounds(1, 2);

	m_InputController.SetTrackedKeystatesBitmask(
		InputController::c_ArrowTrackBit |
		InputController::c_LetterTrackBit |
		InputController::c_SpecialTrackBit
	);

	m_Renderer.Init();
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

	float nearZ = GetUIManager().GetFurthestReservedZ();
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
				GetRenderer().Draw(rCommand.Text, 0, 0, calculatedZcoord, nullptr);
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

	/*
	plan:

	Idea 1: Spam a bunch of sprites to avoid repeating and bleeding. One more variable has been added to the global sprite.
	+	Easy to implement.
	+	Make the rotation a union which includes the raw Z coord and don't use it elsewhere. 
	-	Heavier on memory and performance.

	Req for UI:
	Rendering of UI elements must happen somewhere inside the UI manager.
	The UI manager must own 
		-all Window objects
			-where the order will be read as the windows' ordering, with the first in the array being the closest to the screen
		-a special array of widgets, which will act independantly. 
			-This will allow for windowless buttons, so like general menus or special UI which is always rendered at the lowest level, furthest from us
	UI manager must interpret the most recent GameInput object from InputController and signal if UI has taken over keyboard or mouse input.
	UI manager must interpret input and trigger the appropriate actions.

	New Window and Widgets.
	Window will represent a logical group of Widgets.
	Windows will have unique 32-bit IDs.
	Windows will maintain a position, relative to the screen, but won't have dimensions.
	A window is clicked, when one of its widgets is clicked.
	When a window is clicked, it moves closest to us.
	A window's position is moved when the mouse is HELD and moved, while on a Rect widget.
	A window must have one Rect widget in it's widget array.
	A window must be able to hold an OnClose() function

	Widgets will represent the graphical components of a window.
	Widgets may themselves be composed of other widgets, an example would be a Button being comprised of a Rect and Label.


	
	Plan for UI rework:
	UI manager will have the following dependancies: [ InpuController, Renderer, ResourceService ]
	



	Low prio:
	We should probably check if the Camera type var works as it should inside the renderer and how moving it affects the screen.
	Should probably make the input for the lambda be just a non-const Engine* and pass this. The user can set their own names for ptrs to subsystems
	*/

	/*
	MAKE SEPARATE FACTORIES FOR EACH TYPE THAT NEEDS FACTORIES
	(and make them depend on res service)
	*/

	glfwPollEvents();

	m_InputController.CaptureKeystates();

	//	This shuold probably be moved to InputController and add it as dependancy for UI manager
	//CapturedStates uiCapturedStates = m_UIManager.InterpretInput(m_InputController.ExposeGameInput());
	//
	//if (uiCapturedStates.capturedKeyboard)	m_InputController.ExposeGameInput().SetKeyboardCapturedFlag();
	//if (uiCapturedStates.capturedMouse)		m_InputController.ExposeGameInput().SetMouseCapturedFlag();

	gameLoop(m_ElapsedTimeSeconds, m_InputController.ExposeGameInput(), StoredRenderCommands);

	//	Also, we must allow for other things to be queued for drawing.	//<<< no idea what this means
	QueueFreebatchesToRenderer(StoredRenderCommands);

	//	TODO:	decode the Z value
	m_Renderer.Draw(GetResourceService().GetUIBatch(), 0, 0, 1, nullptr);

	m_Renderer.ExecuteDraws();

	auto end = Clock::now();

	m_ElapsedTimeSeconds = std::chrono::duration<float>(end - start).count();
}