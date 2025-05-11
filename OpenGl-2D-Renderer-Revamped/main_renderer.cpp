
#include "main_renderer.h"









void Renderer2D::Draw(
	const Drawable* _drawable,
	float _xPosition,
	float _yPosition,
	float _zLayer
) {

	m_DrawCallQueue.emplace( _drawable, _xPosition, _yPosition, _zLayer );
}


void Renderer2D::ExecuteDraws() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // some background color
	glClear(GL_COLOR_BUFFER_BIT);

	// SETUP:	COMMON VARIABLES
	GetQuad().Bind();
	size_t DrawCallQueueInitialSize = m_DrawCallQueue.size();

	const Shader* LastShaderUsed = nullptr;

	//	Start executing draw calls
	for (size_t i = 0; i < DrawCallQueueInitialSize; i++) {
		DrawCall DrawCallCurrent = m_DrawCallQueue.top();
		m_DrawCallQueue.pop();

		const SpriteSheet* SheetCurrent = DrawCallCurrent.drawable->GetAsociatedSpriteSheet();
		const Shader* ShaderCurrent = SheetCurrent->GetShader();

		//	SETUP:	COMMON SHEET VARIABLES

		if (LastShaderUsed != ShaderCurrent) {
			LastShaderUsed = ShaderCurrent;
			ShaderCurrent->UseShader();

			ShaderCurrent->SetMat4("u_Projection", m_Camera.GetProjectionMatrix());
			ShaderCurrent->SetMat4("u_View", m_Camera.GetViewMatrix());	
		}

		if (!GetQuad().BufferTexCoords(SheetCurrent)) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, SheetCurrent->GetTextureBufferID());
		}

		ShaderCurrent->SetMat4("u_Model",	//	this is per-instance data, unskippable
			glm::translate(glm::mat4(1.f), DrawCallCurrent.GetPositionVector()));

		ShaderCurrent->SetVec2("u_SpriteOffsets",
			SheetCurrent->GetCalculatedSpriteOffsets(DrawCallCurrent.drawable->GetSpriteIndex()));


		if (!DrawCallCurrent.drawable->HasForcedDimensions()) {
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr));
			continue;
		}
		else {
			//	terrible idea
			//	would probably be better to bind a temporary, dynamically buffered
			//	VBO with the forced coords, call glDrawElements() and rebind the 
			//	standard VBO from StandardQuad

			unsigned int StandardVBO = GetQuad().m_VextexBuffer;
			
			unsigned int NewXValue = DrawCallCurrent.drawable->GetForcedXValue() != -1 ? DrawCallCurrent.drawable->GetForcedXValue() : GetQuad().m_StandardSpritePixelLength;
			unsigned int NewYValue = DrawCallCurrent.drawable->GetForcedYValue() != -1 ? DrawCallCurrent.drawable->GetForcedYValue() : GetQuad().m_StandardSpritePixelLength;

			unsigned int  TEMP_buffer[] = {
				0,								0,
				0,								NewYValue,
				NewXValue,						NewYValue,
				NewXValue,						0
			};
				
			glBindBuffer(GL_ARRAY_BUFFER, StandardVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TEMP_buffer), TEMP_buffer);

			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr));

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TEMP_buffer), GetQuad().g_stdVertexCoordArray);
		}

	}

	//--	CLEAN UP	--//

	glBindTexture(GL_TEXTURE_2D, 0);
	GetQuad().Unbind();

	glfwSwapBuffers(GetWinHandle());
	glfwPollEvents();
}


bool Renderer2D::Init() {

	if (!glfwInit()) {
		std::cout << "Window not opened.\n";
		return false;
	}

	m_MainWindowHandle = glfwCreateWindow(
		m_ScreenWidth,
		m_ScreenHeight,
		m_WindowTitle.c_str(),
		NULL, NULL
	);

	if (!m_MainWindowHandle) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_MainWindowHandle);
	glfwSwapInterval(1);
	GLenum glewReturnCode = glewInit();
	if (glewReturnCode != GLEW_OK) {
		std::cout << glewGetErrorString(glewReturnCode);
		return false;
	}

	m_StandardQuad.Init();

	m_Camera.Initialisation(
		{ 1, 1 },
		{ m_ScreenWidth, m_ScreenHeight }
	);


	return true;
}


Renderer2D::Renderer2D(
	int _screenWidth,
	int _screenHeight,
	const char* _windowTitle,
	bool _fullscreen
)
	: m_ScreenWidth(_screenWidth),
	m_ScreenHeight(_screenHeight),
	m_WindowTitle(_windowTitle),
	m_Fullscreen(_fullscreen)
{}

void Renderer2D::LoadShader(
	const std::string& _locationShaderFile,
	const std::string& _shaderName
) {
	m_ShaderArray.emplace_back(
		_locationShaderFile,
		_shaderName
	);
}


void Renderer2D::LoadSpriteSheet(
	const std::string& _locationRawImage,
	const std::string& _sheetName,
	const Shader* _preferredShader,
	int _spritesPerRow,
	int _spritesPerCol
) {
	m_SpriteSheetArray.emplace_back(
		_locationRawImage,
		_sheetName,
		_preferredShader,
		_spritesPerRow,
		_spritesPerCol
	);
}

SpriteSheet* Renderer2D::GetSpriteSheetByName(
	const char* _spriteSheetName
) {
	size_t len = m_SpriteSheetArray.size();
	for (size_t i = 0; i < len; i++) {
		if (
			strcmp(_spriteSheetName, m_SpriteSheetArray[i].GetName().c_str()) == 0
			) {
			return &m_SpriteSheetArray[i];
		}
	}
	return nullptr;
}

Shader* Renderer2D::GetShaderByName(
	const char* _shaderName
) {
	size_t len = m_ShaderArray.size();
	for (size_t i = 0; i < len; i++) {
		if (
			strcmp(_shaderName, m_ShaderArray[i].GetName().c_str()) == 0
			) {
			return &m_ShaderArray[i];
		}
	}
	return nullptr;
}


void StandardQuad::Unbind() {
	glBindVertexArray(0);
}

void StandardQuad::Init() {

	

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VextexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VextexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_stdVertexCoordArray), g_stdVertexCoordArray, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_UNSIGNED_INT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_TexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_stdTexCoordArray), g_stdTexCoordArray, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_stdIndexArray), g_stdIndexArray, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void StandardQuad::Bind() {
	glBindVertexArray(m_VAO);
}

bool StandardQuad::BufferTexCoords(
	const SpriteSheet* _spriteSheet
) {
	const UVRegion Region = (_spriteSheet)->GetSheetSpriteUVregion();

	if (Region == GetCurrentUVregion()) return true;

	float texCoords[8] = {
		Region.u0, Region.v0,
		Region.u0, Region.v1,
		Region.u1, Region.v1,
		Region.u1, Region.v0
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texCoords), texCoords);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_UVregionCurrentlyUsed = Region;
	return false;
}

