


#include "main_renderer.h"



int main() {

	Renderer2D r(1600, 900, "Absolyuten hui");

	r.Init();


	r.LoadShader(
		"C:\\Users\\zapza\\source\\repos\\OpenGl-2D-Renderer-Revamped\\OpenGl-2D-Renderer-Revamped\\test\\res\\test.shader",
		"test_shader"
	);
	 
	r.LoadSpriteSheet(
		"C:\\Users\\zapza\\Desktop\\test.png",
		"test_sheet",
		r.GetShaderByName("test_shader"),
		1, 1
	);

	Drawable a(
		r.GetSpriteRegionByIndex(0)
	);


	while (true) {

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // some background color
		glClear(GL_COLOR_BUFFER_BIT);


		r.Draw(a, 100, 100);


		glfwSwapBuffers(r.GetWinHandle());
		glfwPollEvents();
	}

	return 0;
}
