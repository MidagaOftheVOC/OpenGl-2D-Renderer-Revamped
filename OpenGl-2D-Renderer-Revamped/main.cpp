


#include "main_renderer.h"

#include "components/batch_types/batch_instance_primitives.h"

#include "test/test_snippets.h"

int main(int argc, char** argv) {

	Renderer2D r(1600, 900, "Absolyuten hui");
	r.Init();
	//r.SetBaseDirectory(__FILE__);


	r.UploadShaderParameters("test\\res\\text.shader",				r.c_SpecialTextShaderName);
	r.UploadShaderParameters("test\\res\\batch_test.shader",		"strict_batch");
	r.UploadShaderParameters("test\\res\\sb_std.shader",			"soft_batch");
	r.UploadShaderParameters("test\\res\\free_batch.shader",		"fb_shader");	//	W/O UBO
	r.UploadShaderParameters("test\\res\\sb_floating_quads.shader",	"sb_floating_quads");
	r.UploadShaderParameters("test\\res\\fb_std.shader",			"fd_std");		//	WITH UBO
	r.UploadShaderParameters("test\\res\\uib_std.shader",			"uib_std");	


	r.UploadSpriteSheetParameters("test\\res\\cyrillic.png",		"test_font",				r.c_SpecialTextShaderName,	9, 9);
	r.UploadSpriteSheetParameters("test\\res\\test.cfg",			"fox",						"strict_batch",				0, 0);
	r.UploadSpriteSheetParameters("test\\res\\test.cfg",			"soft_sheet",				"soft_batch",				0, 0);
	r.UploadSpriteSheetParameters("test\\res\\test.cfg",			"fb_sheet",					"fb_shader",				0, 0);
	r.UploadSpriteSheetParameters("test\\res\\test.cfg",			"sb_fq",					"sb_floating_quads",		0, 0);
	r.UploadSpriteSheetParameters("test\\res\\panda.cfg",			"sb_panda",					"sb_floating_quads",		0, 0);
	r.UploadSpriteSheetParameters("test\\res\\test.cfg",			"fd_std1",					"fd_std",					0, 0);
	r.UploadSpriteSheetParameters("test\\res\\panda.cfg",			"fd_std2",					"fd_std",					0, 0);
	r.UploadSpriteSheetParameters("test\\res\\gui.cfg",				"uib_std",					"uib_std",					0, 0);
	r.UploadSpriteSheetParameters("test\\res\\gui.cfg",				r.c_SpecialUISheetName,		"uib_std",					0, 0);

	
	r.StartLoadingProcess();


	InputController& input = r.GetInputController();
	
	UIManager& ui = r.GetUIManager();
	
	Window win  = ui.CreateWindow({	600, 300 },	20);
	Window win2 = ui.CreateWindow({ 400, 200 }, 20);


	Label label = Label(r.GenText(U"здрасти"), {50, 80});
	Label label2 = Label(r.GenText(U"бепче"), { 50, 100 });
	win.AttachWidget(&label);
	win.AttachWidget(&label2);

	ID winID1 = ui.AddWindow(win);
	ID winID2 = ui.AddWindow(win2);

	ui.OpenWindow(winID1, 200, 200);
	ui.OpenWindow(winID2, 360, 250);

	while (!r.IsRunning()) {
		input.CaptureKeystates();

		//r.Draw(&rando,	300, 700, 1, nullptr);

		if (input.IsHeld(GLFW_KEY_RIGHT)) {
			std::cout << "right pressed\n";
		}

		if (input.IsPressed(GLFW_KEY_ESCAPE)) {

			return 0;
		}

		r.ExecuteDraws();

	}

	return 0;
}
