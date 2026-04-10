#include "engine.h"

#include <print>

int main(int argc, char** argv) {

	Engine2D eng(1600, 900, "nog", false);
	auto& resService = eng.GetResourceService();

	resService.UploadShaderParameters("test\\res\\text.shader",					resService.c_SpecialTextShaderName);
	resService.UploadShaderParameters("test\\res\\fb_std.shader",				"fd_std");		//	WITH UBO
	resService.UploadShaderParameters("test\\res\\uib_std.shader",				"uib_std");


	resService.UploadSpriteSheetParameters("test\\res\\cyrillic.png",			"test_font",						resService.c_SpecialTextShaderName, 9, 9);
	resService.UploadSpriteSheetParameters("test\\res\\test.cfg",				"fd_std1",							"fd_std",							0, 0);
	
	resService.UploadSpriteSheetParameters("test\\res\\panda.cfg",				"fd_std2",							"fd_std",							0, 0);
	
	resService.UploadSpriteSheetParameters("test\\res\\gui.cfg",				"uib_std",							"uib_std",							0, 0);
	resService.UploadSpriteSheetParameters("test\\res\\gui.cfg",				resService.c_SpecialUISheetName,	"uib_std",							0, 0);

	//	This is terrible, but it's supposed to represent just a skin from the first sprite sheet loaded in the special UI batch for the UI manager
	//	and the first 9 sprites defined inside it.
	PaneSkin skin;
	skin.m_Name = "default";
	SpriteInformation SIarray[] = {
		{0, 0},
		{0, 1},
		{0, 2},
		{0, 3},
		{0, 6},
		{0, 7},

		{0, 4},
		{0, 5},

		{0, 8}
	};

	eng.Init();

	memcpy(skin.m_SIArray, SIarray, 9 * sizeof(SpriteInformation));
	
	eng.GetResourceService().AddSkin(skin);

	Batch freebatch = Batch(true);
	freebatch.AddSheetToBatch(eng.GetResourceService().GetSpriteSheetByName("fd_std2"));
	freebatch.BufferUBOs();

	float x = 150;
	float y = 101;
	float Rotation = 0.f;
	
	SpriteInstance instance = freebatch.GetSprite("fd_std2", "nose");

	constexpr float oneDef = 1.f / 3.1418f;

	while (eng.IsRunning()) {
		eng.ExecuteFrame([&](float elapsedTimeSeconds, GameInput input, GameLoopReturnType& renderComms) {
			renderComms.QueueRenderObject(&freebatch, 2);

			if (input.IsHeld(GLFW_KEY_LEFT)) {
				x -= 1;
				std::println("NEW X: {}", x);
			}

			if (input.IsHeld(GLFW_KEY_RIGHT)) {
				x += 1;
				std::println("NEW X: {}", x);
			}

			if (input.IsHeld(GLFW_KEY_R)) {
				Rotation += oneDef;
			}

			if (input.IsHeld(GLFW_KEY_DOWN)) {
				instance.dimensions.x += 5;
			}

			freebatch.DrawSpriteInstance(instance, x, y, Rotation);
			freebatch.DrawSpriteInstance(instance, x + 50, y + 50, Rotation);
		});
	}

	return 0;
}


/*
	InputController& input = eng.GetInputController();

	UIManager& ui = eng.GetUIManager();

	Window win  = ui.CreateWindow({	600, 300 },	20);
	Window win2 = ui.CreateWindow({ 400, 200 }, 20);


	Label label = Label(eng.GetTextFactory().GenerateText(U"здрасти"), {50, 80});
	Label label2 = Label(eng.GetTextFactory().GenerateText(U"бепче"), { 50, 100 });

	Button btn = Button({ 30, 30 }, eng.GetTextFactory().GenerateText(U"бутон"), eng.GetPaneFactory().CreatePane(100, 50, 2, "default"), []() {
		std::cout << "Natisnat buton\n";
	});

	win.AttachWidget(&label);
	win.AttachWidget(&label2);
	win2.AttachWidget(&btn);

	ID winID1 = ui.AddWindow(win);
	ID winID2 = ui.AddWindow(win2);

	ui.OpenWindow(winID1, 200, 200);
	ui.OpenWindow(winID2, 560, 250);
*/