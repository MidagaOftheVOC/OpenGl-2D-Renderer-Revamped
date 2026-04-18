#include "engine.h"
#include "components/ui/widget/widgets/label.h"
#include "components/ui/widget/widgets/window.h"

#include <print>

int main(int argc, char** argv) {

	Engine2D eng(1600, 900, "nog", false);
	auto& resService = eng.GetResourceService();

	resService.UploadShaderParameters("test\\res\\text.shader",			resService.c_SpecialTextShaderName);
	resService.UploadShaderParameters("test\\res\\batch.shader",		"batch");		//	WITH UBO
	resService.UploadShaderParameters("test\\res\\batch_ui.shader",		"batch_ui");


	resService.UploadSpriteSheetParameters("test\\res\\cyrillic.png",	"cyrillic",							resService.c_SpecialTextShaderName, 9, 9);
	
	resService.UploadSpriteSheetParameters("test\\res\\test.cfg",		"testSheet",						"batch",							0, 0);
	resService.UploadSpriteSheetParameters("test\\res\\panda.cfg",		"pandaSheet",						"batch",							0, 0);
	
	resService.UploadSpriteSheetParameters("test\\res\\gui.cfg",		resService.c_SpecialUISheetName,	"batch_ui",							0, 0);

	eng.Init();
	
	const bool INIT_OPENGL_OBJECTS = true;
	Batch freebatch = Batch(INIT_OPENGL_OBJECTS);
	freebatch.AddSheetToBatch(eng.GetResourceService().GetSpriteSheetByName("testSheet"));
	freebatch.AddSheetToBatch(eng.GetResourceService().GetSpriteSheetByName("pandaSheet"));
	freebatch.BufferUBOs();

	float x = 150;
	float y = 101;
	float Rotation = 0.f;
	
	SpriteInstance instance = freebatch.GetSprite("pandaSheet", "nose");
	SpriteInstance eye = freebatch.GetSprite("pandaSheet", "eye");
	SpriteInstance testedInstance = freebatch.GetSprite("testSheet", "nose");

	constexpr float oneDef = 1.f / 3.1418f;

	TextOptions options;
	options.font = &resService.GetDefaultFont();

	Text txt = Text(
		U"едно",
		options
	);

	Text txt2 = Text(
		U"две",
		options
	);

	auto label = std::make_unique<Label>(txt, glm::vec2(10.f, 10.f), glm::vec2(200, 50));
	auto window = std::make_unique<Window>(1, glm::vec2(400, 400), glm::vec2(300, 200), resService.GetSkinByName("default"));

	window.get()->AddChild(std::move(label));
	window.get()->SetOnClick([]() {
		std::println("nogg 1");
	});


	auto label2 = std::make_unique<Label>(txt2, glm::vec2(10.f, 10.f), glm::vec2(200, 50));
	auto window2 = std::make_unique<Window>(2, glm::vec2(450, 380), glm::vec2(300, 200), resService.GetSkinByName("default"));

	window2.get()->AddChild(std::move(label2));
	window2.get()->SetOnClick([]() {
		std::println("nogg 2");
	});

	eng.GetUIManager().OpenWindow(std::move(window));
	eng.GetUIManager().OpenWindow(std::move(window2));

	while (eng.IsRunning()) {
		eng.ExecuteFrame([&](float elapsedTimeSeconds, GameInput input, GameLoopReturnType& renderComms) {
			renderComms.QueueRenderObject(&freebatch, 2);

			//std::println("FPS: {:.0f}", 1 / elapsedTimeSeconds);

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

			freebatch.DrawSprite(instance, x, y, Rotation);
			freebatch.DrawSprite(instance, x + 50, y + 50, 0);
			freebatch.DrawSprite(testedInstance, x + 100, y + 200, 0);
			freebatch.DrawSprite(eye, x, y + 200, Rotation);
		});
	}

	return 0;
}