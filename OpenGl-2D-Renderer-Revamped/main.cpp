


#include "main_renderer.h"

#include "components/batch_types/batch_instance_primitives.h"

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

	r.UploadSpriteSheetParameters(
		"test\\res\\panda.cfg",
		"sb_panda",
		"sb_floating_quads",
		0, 0
	);


	

	r.UploadShaderParameters(
		"test\\res\\fb_std.shader",
		"fd_std"
	);

	r.UploadSpriteSheetParameters(
		"test\\res\\test.cfg",
		"fd_std1",
		"fd_std",
		0, 0
	);

	r.UploadSpriteSheetParameters(
		"test\\res\\panda.cfg",
		"fd_std2",
		"fd_std",
		0, 0
	);



	
	r.UploadSpriteSheetParameters(
		"test\\res\\gui.cfg",
		"uib_std",
		"uib_std",
		0, 0
	);


	r.UploadShaderParameters(
		"test\\res\\uib_std.shader",
		"uib_std"
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

	Text text = Text(
		&f,
		U"здравейте, дами и господа!",
		textoptions
	);

	t.SetWordWrapBound(200);

	InputController& input = r.GetInputController();


	const unsigned short indices[] = { 0, 0, 0, 0 };
	float rotations[] = {0.f, 1.f, 2.f, 3.f};
	float PositionPais[] = { 20.f, 20.f, 120.f, 120.f, 220.f, 220.f, 320.f, 320.f };

	const SpriteSheet* SB_normal = r.GetSpriteSheetByName("sb_fq");
	const SpriteSheet* SB_panda = r.GetSpriteSheetByName("sb_panda");


	SoftBatch fq = SoftBatch(4, SoftBatchType::FloatingQuad);
	fq.InitialiseBuffers();
	fq.AddSheetToBatch(
		SB_normal
	);

	fq.AddSheetToBatch(
		SB_panda
	);
	fq.BufferUBOs();

	fq.UpdateBuffers(
		nullptr,
		rotations,
		PositionPais,
		4
	);

	unsigned short sprites[] = { 0, 0, 0, 0 };
	unsigned short sheets[] = { 0, 0, 1, 1 };

	fq.UpdateSpriteIndexVBO(sprites, sheets, 4);

	StrictBatch strict = GetInitialisedStrictBatch(r.GetSpriteSheetByName("fox"));


	const SpriteSheet* FB_normal = r.GetSpriteSheetByName("fd_std1");
	const SpriteSheet* FB_panda = r.GetSpriteSheetByName("fd_std2");

	FreeBatch free = FreeBatch(4);
	free.InitialiseBuffers();
	free.AddSheetToBatch(FB_normal);
	free.AddSheetToBatch(FB_panda);
	free.BufferUBOs();
	

	SpriteInformation SI[4] = {
		{0, 0}, {0, 0}, {1, 0}, {1, 0}
	};
	float dims[] = { 50.f, 50.f, 100.f, 100.f, 200.f, 250.f, 150.f, 150.f};
	free.UpdateBuffers(SI, rotations, PositionPais, dims, 4);




	UIBatch ui(9);
	ui.InitialiseBuffers();
	const SpriteSheet* sheet = r.GetSpriteSheetByName("uib_std");
	ui.AddSheetToBatch(sheet);
	ui.BufferUBOs();

	float uidims[] = {
		20, 20, // ul_corner
		20, 20, // ur_corner
		20, 20, // ll_corner
		20, 20, // lr_corner

		20, 20, // leftborder
		20, 20, // rightborder
		20, 20, // upperborder
		20, 20, // lowerborder

		20, 20  // middle
	};


	SpriteInformation uiSI[] = {
		{0, 0},
		{0, 1},
		{0, 2},
		{0, 3},
		{0, 4},
		{0, 5},
		{0, 6},
		{0, 7},
		{0, 8}
	};


	float uiPositions[] = {
		0, 0,        
		40, 0,       
		0, 40,       
		40, 40,      

		0, 20,       
		40, 20,      
		20, 0,       
		20, 40,      

		20, 20       
	};



	float zLayer = 5.f;

	ui.UpdateBuffers(
		uiSI,
		uiPositions,
		uidims,
		&zLayer,
		9
	);


	while (!r.IsRunning()) {
		input.CaptureKeystates();

		//r.Draw(&fq, 100, 100, 3, nullptr);
		//r.Draw(&free, 800, 400, 2, nullptr);
		//r.Draw(&t, 300, 300, 2, nullptr);
		//r.Draw(&text, 300, 500, 2, nullptr);

		r.Draw(&ui, 0, nullptr);

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
