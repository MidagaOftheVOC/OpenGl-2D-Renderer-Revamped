


#include "main_renderer.h"



int main() {

	Renderer2D r(1600, 900, "Absolyuten hui");

	r.Init();

	r.UploadShaderParameters(
		"test\\res\\text.shader",
		r.c_SpecialTextShaderName
	);

	r.UploadSpriteSheetParameters(
		"test\\res\\cyrillic.png",
		"test_font",
		r.c_SpecialTextShaderName,
		9, 9
	);


	r.UploadSpriteSheetParameters(
		"test\\res\\test.cfg",
		"fox",
		"strict_batch",
		0, 0
	);

	r.UploadShaderParameters(
		"test\\res\\batch_test.shader",
		"strict_batch"
	);


	r.UploadShaderParameters(
		"test\\res\\softbatch_test.shader",
		"soft_batch"
	);

	r.UploadSpriteSheetParameters(
		"test\\res\\test.cfg",
		"soft_sheet",
		"soft_batch",
		0, 0
	);


	r.UploadShaderParameters(
		"test\\res\\free_batch.shader",
		"fb_shader"
	);

	r.UploadSpriteSheetParameters(
		"test\\res\\test.cfg",
		"fb_sheet",
		"fb_shader",
		0, 0
	);


	
	r.StartLoadingProcess();


	Font f = Font(
		r.GetSpriteSheetByName("test_font"),
		"cyrillic"
	);

	unsigned short off[] = { 
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
		20, 20, 20, 20, 20, 20 };
	f.Init(
		U"абвгдежзийклмнопрстуфхцчшщъьюяѝ .,+-!?;:&><#/",
		off,
		50
	);

	TextOptions textoptions(20, 5);

	Text t = Text(
		&f,
		U"здрастиииииии! всичките сте курви!",
		textoptions
	);

	t.SetWordWrapBound(200);

	InputController& input = r.GetInputController();


	int indices[] = { 0, 0, 0, 0 };
	float rotations[] = {0.f, 1.f, 2.f, 3.f};
	float PositionPais[] = { 20.f, 20.f, 120.f, 120.f, 220.f, 220.f, 320.f, 320.f };

	
	SoftBatch soft = SoftBatch(r.GetSpriteSheetByName("soft_sheet"), 4);
	soft.InitialiseBuffers();

	soft.UpdateBuffers(
		indices,
		rotations,
		PositionPais,
		4
	);




	FreeBatch  free = FreeBatch(r.GetSpriteSheetByName("fb_sheet"), 4);

	free.InitialiseBuffers();

	free.UpdateBuffers(
		indices,
		rotations,
		PositionPais,
		4
	);


	while (!r.IsRunning()) {
		input.CaptureKeystates();


		//r.Draw(&t, 300, 300, 2, nullptr);

		//r.Draw(&soft, 100, 100, 2, nullptr);

		r.Draw(&free, 300, 300, 2, nullptr);

		if (input.IsHeld(GLFW_KEY_RIGHT)) {
			t.SetWordWrapBound(t.GetRightWordWrapBound() + .5f);
			std::cout << "right pressed\n";
		}

		if (input.IsPressed(GLFW_KEY_ESCAPE)) {
			return 0;
		}


		r.ExecuteDraws();
	}

	return 0;
}
