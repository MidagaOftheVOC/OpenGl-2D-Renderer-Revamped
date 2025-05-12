
#include "main_renderer.h"






Drawable Renderer2D::GetDrawable(
	const char* _spriteSheetName,
	int _indexInSpriteSheet
) {
	SpriteSheet* SheetOfTarget = GetSpriteSheetByName(_spriteSheetName);

	DEBUG_ASSERT(SheetOfTarget != nullptr, "Indexing Drawable from non-existant SpriteSheet object or name.");

	return Drawable(SheetOfTarget, _indexInSpriteSheet);
}


void Renderer2D::ExtractDrawablesFromSheets(
	std::vector<Drawable> OUT_drawableVector
) {
	for (size_t i = 0; i < m_SpriteSheetArray.size(); i++) {

	}
}


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

	GetQuad().Bind();
	const Shader* LastUsedShader = nullptr;
	const SpriteSheet* LastUsedSheet = nullptr;

	int size = m_DrawCallQueue.size();
	for(int i = 0; i < size; i++)
	{
		DrawCall DrawcallCurrent = m_DrawCallQueue.top();
		m_DrawCallQueue.pop();

		const SpriteSheet* sheet;
		const Shader* shader;

		sheet = DrawcallCurrent.drawable->GetAsociatedSpriteSheet();
		shader = sheet->GetShader();

		if (LastUsedSheet != sheet) {
			LastUsedSheet = sheet;
			glBindTexture(GL_TEXTURE_2D, sheet->GetTextureBufferID());
			GetQuad().BufferTexCoords(sheet);
		}

		if(LastUsedShader != shader){
			LastUsedShader = shader;
			shader->UseShader();
			shader->SetMat4("u_Projection", m_Camera.GetProjectionMatrix());
			shader->SetMat4("u_View", m_Camera.GetViewMatrix());
		}

		//	per-instance data, unskippable
		shader->SetMat4("u_Model", glm::translate(glm::mat4(1.f), DrawcallCurrent.GetPositionVector()));
		shader->SetVec2("u_SpriteOffsets",
			sheet->GetCalculatedSpriteOffsets(DrawcallCurrent.drawable->GetSpriteIndex()));

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr));
	}

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

