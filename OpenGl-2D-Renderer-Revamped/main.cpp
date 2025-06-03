


#include "main_renderer.h"


#include "test/test_snippets.h"

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
		"test\\res\\sb_std.shader",
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



	r.UploadShaderParameters(
		"test\\res\\sb_floating_quads.shader",
		"sb_floating_quads"
	);

	r.UploadSpriteSheetParameters(
		"test\\res\\test.cfg",
		"sb_fq",
		"sb_floating_quads",
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


	const unsigned short indices[] = { 0, 0, 0, 0 };
	float rotations[] = {0.f, 1.f, 2.f, 3.f};
	float PositionPais[] = { 20.f, 20.f, 120.f, 120.f, 220.f, 220.f, 320.f, 320.f };

	
	SoftBatch soft = SoftBatch(4);
	soft.InitialiseBuffers();
	soft.AddSheetToBatch(
		r.GetSpriteSheetByName("soft_sheet")
	);

	soft.UpdateBuffers(
		nullptr,
		rotations,
		PositionPais,
		4
	);

	soft.UpdateSpriteIndexVBO(indices, 0, 4);


	SoftBatch fq = SoftBatch(4, SoftBatchType::FloatingQuad);
	fq.InitialiseBuffers();
	fq.AddSheetToBatch(
		r.GetSpriteSheetByName("sb_fq")
	);

	fq.UpdateBuffers(
		nullptr,
		rotations,
		PositionPais,
		4
	);

	fq.UpdateSpriteIndexVBO(indices, 0, 4);




	StrictBatch strict = GetInitialisedStrictBatch(r.GetSpriteSheetByName("fox"));

	while (!r.IsRunning()) {
		input.CaptureKeystates();

		r.Draw(&strict, 70, 70, 2, nullptr);

		r.Draw(&t, 300, 300, 2, nullptr);

		//r.Draw(&soft, 100, 100, 2, nullptr);

		r.Draw(&fq, 100, 100, 3, nullptr);

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
