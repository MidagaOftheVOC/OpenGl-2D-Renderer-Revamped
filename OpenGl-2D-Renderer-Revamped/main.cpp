


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

	
	r.StartLoadingProcess();


	Drawable d = r.GenerateDrawable("test_sheet", 0);
	Drawable c = r.GenerateDrawable("test_sheet", 1);
	
	UniformDataVector test;


	StrictBatch b = StrictBatch(r.GetSpriteSheetByName("test_batch_sheet"), 4);

	
	int arr[] = {0, 1, 2 , 3};

	b.InitialiseBuffers(arr);




	SoftBatch s = SoftBatch(
		r.GetSpriteSheetByName("test_sbatch"),
		4
	);

	int indicies[] = { 0, 1, 2, 3 };

	float rotations[] = { 1.f, 2.f, 3.f, 4.f };

	float positions[] = {
		400, 50,
		400, 200,
		600, 50,
		600, 200
	};

	s.InitialiseBuffers(indicies, rotations, positions);

	InputController& input = r.GetInputController();

	while (!r.IsRunning()) {

		input.CaptureKeystates();

		r.Draw(&d, 100, 100, 2, nullptr);
		r.Draw(&c, 200, 100, 2, nullptr);

		//r.Draw(&b, 400, 400, 2, 2, nullptr);


		r.Draw(&s, 0, 0, 2, nullptr);

		if (input.IsReleased(GLFW_KEY_A))
			std::cout << "A\n";

		r.ExecuteDraws();
	}

	return 0;
}
