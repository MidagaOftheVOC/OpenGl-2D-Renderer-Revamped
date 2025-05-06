
#include "main_renderer.h"









void Renderer2D::Draw(
	Drawable _drawable,
	int _xPosition,
	int _yPosition
) {

	const Shader* shaderToUse = _drawable.GetSpriteRegion()->ShaderPtr;

	shaderToUse->UseShader();


	shaderToUse->SetMat4("u_Projection", m_Camera.GetProjectionMatrix());
	shaderToUse->SetMat4("u_View", m_Camera.GetViewMatrix());
	shaderToUse->SetMat4("u_Model", glm::translate(glm::mat4(1.f), glm::vec3(_xPosition, _yPosition, +2.f)));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _drawable.GetSpriteRegion()->TextureID);

	GetQuad().BufferTexCoords(_drawable.GetSpriteRegion());
	GetQuad().Bind();

	CheckGLErrors();
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr));
	CheckGLErrors();

	GetQuad().Unbind();
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
	m_SpriteSheetArray.back().ExtractSpriteRegionsToArray(m_SpriteRegionArray);
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

	unsigned int  g_stdVertexCoordArray[] = {
		0,								0,
		0,								m_StandardSpritePixelLength,
		m_StandardSpritePixelLength,	m_StandardSpritePixelLength,
		m_StandardSpritePixelLength,	0
	};

	float g_stdTexCoordArray[] = {
		0.f,	0.f,
		0.f,	1.f,
		1.f,	1.f,
		1.f,	0.f
	};

	unsigned short g_stdIndexArray[] = {
		0,	1,	2,
		0,	2,	3
	};


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

void StandardQuad::BufferTexCoords(
	const SpriteRegion* _spriteRegion
) {
	float texCoords[8] = {
		_spriteRegion->Region.u0, _spriteRegion->Region.v0,
		_spriteRegion->Region.u0, _spriteRegion->Region.v1,
		_spriteRegion->Region.u1, _spriteRegion->Region.v1,
		_spriteRegion->Region.u1, _spriteRegion->Region.v0
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texCoords), texCoords);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

