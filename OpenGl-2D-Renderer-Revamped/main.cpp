#include "engine.h"

int main(int argc, char** argv) {

	Engine2D eng(1600, 900, "nog", false);
	auto& resService = eng.GetResourceService();

	resService.UploadShaderParameters("test\\res\\text.shader",					resService.c_SpecialTextShaderName);
	resService.UploadShaderParameters("test\\res\\batch_test.shader",			"strict_batch");
	resService.UploadShaderParameters("test\\res\\sb_std.shader",				"soft_batch");
	resService.UploadShaderParameters("test\\res\\free_batch.shader",			"fb_shader");	//	W/O UBO
	resService.UploadShaderParameters("test\\res\\sb_floating_quads.shader",	"sb_floating_quads");
	resService.UploadShaderParameters("test\\res\\fb_std.shader",				"fd_std");		//	WITH UBO
	resService.UploadShaderParameters("test\\res\\uib_std.shader",				"uib_std");


	resService.UploadSpriteSheetParameters("test\\res\\cyrillic.png",			"test_font",						resService.c_SpecialTextShaderName, 9, 9);
	resService.UploadSpriteSheetParameters("test\\res\\test.cfg",				"fox",								"strict_batch",						0, 0);
	resService.UploadSpriteSheetParameters("test\\res\\test.cfg",				"soft_sheet",						"soft_batch",						0, 0);
	resService.UploadSpriteSheetParameters("test\\res\\test.cfg",				"fb_sheet",							"fb_shader",						0, 0);
	resService.UploadSpriteSheetParameters("test\\res\\test.cfg",				"sb_fq",							"sb_floating_quads",				0, 0);
	resService.UploadSpriteSheetParameters("test\\res\\panda.cfg",				"sb_panda",							"sb_floating_quads",				0, 0);
	resService.UploadSpriteSheetParameters("test\\res\\test.cfg",				"fd_std1",							"fd_std",							0, 0);
	resService.UploadSpriteSheetParameters("test\\res\\panda.cfg",				"fd_std2",							"fd_std",							0, 0);
	resService.UploadSpriteSheetParameters("test\\res\\gui.cfg",				"uib_std",							"uib_std",							0, 0);
	resService.UploadSpriteSheetParameters("test\\res\\gui.cfg",				resService.c_SpecialUISheetName,	"uib_std",							0, 0);

	
	eng.Init();

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

	memcpy(skin.m_SIArray, SIarray, 9 * sizeof(SpriteInformation));
	
	eng.GetResourceService().AddSkin(skin);

	/*
	
		IMPROVE BATCHES
		They should know when to update their buffers. They should have a fn DrawSprite(...) or something which takes SpriteInfo or something.
		Before rendering, update arrays if needed.

		Also make a struct to unify those 1-4 OUT_ arrays to make them easier to use.
	
		MAKE SEPARATE FACTORIES FOR EACH TYPE THAT NEEDS FACTORIES
		(and make them depend on res service)
	*/


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

	while (eng.IsRunning()) {
		input.CaptureKeystates();
		//input.CaptureMouseButtons();

		//r.Draw(&rando,	300, 700, 1, nullptr);

		if (input.IsHeld(GLFW_KEY_RIGHT)) {
			std::cout << "right pressed\n";
		}

		if (input.IsPressed(GLFW_KEY_ESCAPE)) {

			return 0;
		}

		//if(r.GetInputController().IsHeld(GLFW_MOUSE_BUTTON_1)) {		std::cout << "Mouse is   HELD	\n"; }
		//if (r.GetInputController().IsPressed(GLFW_MOUSE_BUTTON_1)) {	std::cout << "Mouse is   PRESSED\n"; }
		//if (r.GetInputController().IsReleased(GLFW_MOUSE_BUTTON_1)) {	std::cout << "Mouse is   RELEASED\n"; }

		

		r.ExecuteDraws();
	}

	return 0;
}
