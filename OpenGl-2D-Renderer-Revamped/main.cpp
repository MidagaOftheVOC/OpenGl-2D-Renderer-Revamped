


#include "main_renderer.h"



int main() {

	Renderer2D r(1600, 900, "Absolyuten hui");

	r.Init();

	r.UploadShaderParameters(
		"test\\res\\test.shader",
		"test_shader"
	);

	r.UploadShaderParameters(
		"test\\res\\batch_test.shader",
		"test_batch"
	);
 
	try {
		r.UploadSpriteSheetParameters(
			"test\\res\\test.png",
			"test_sheet",
			"test_shader",
			2, 2
		);
		r.UploadSpriteSheetParameters(
			"test\\res\\test.png",
			"test_batch_sheet",
			"test_batch",
			2, 2
		);
	}
	catch (std::exception ex) {
		std::cout << ex.what();
	}

	r.UploadShaderParameters(
		"test\\res\\softbatch_test.shader",
		"test_soft_batch"
	);

	r.UploadSpriteSheetParameters(
		"test\\res\\test.png",
		"test_sbatch",
		"test_soft_batch",
		2, 2
	);


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
		U"здрастиииииии! всичките сте курви!",// всичките сте курви!",
		textoptions
	);

	t.SetWordWrapBound(200);

	t.CalculateWordWraps();

	InputController& input = r.GetInputController();


	StrictBatch b = StrictBatch(r.GetSpriteSheetByName("test_batch_sheet"), 4);


	int arr[] = { 0, 1, 2 , 3 };

	b.InitialiseBuffers(arr);


	while (!r.IsRunning()) {
		input.CaptureKeystates();

		r.Draw(&b, 300, 100, 2, 2, nullptr);

		r.Draw(&t, 300, 300, 2, nullptr);

		if (input.IsPressed(GLFW_KEY_ESCAPE)) {
			return 0;
		}

		r.ExecuteDraws();
	}

	return 0;
}
