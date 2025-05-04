#include "sprite_sheet.h"






SpriteSheet::SpriteSheet(
	const std::string& _locationRawImage,
	const std::string& _sheetName,
	const Shader* _preferredShader,
	int _spritesPerRow,
	int _spritesPerCol
) 
	: m_SheetName(_sheetName),
	m_Shader(_preferredShader),
	m_SpriteCountPerCol(_spritesPerCol),
	m_SpriteCountPerRow(_spritesPerRow)
{

	unsigned char* imageData = nullptr;
	int nChannels;
	imageData = stbi_load(
		_locationRawImage.c_str(),
		&m_SheetWidth,
		&m_SheetHeight,
		&nChannels,
		4
	);

	if (!imageData) {
		// unfixable, crash here
		
		throw std::exception(stbi_failure_reason());
		
		//throw std::exception(std::format("Failed to load sprite sheet \'{}\' at {} : {}",
		//	m_SheetName, _locationRawImage, stbi_failure_reason()));
	}

	glGenTextures(1, &m_TextureBufferID);
	glBindTexture(GL_TEXTURE_2D, m_TextureBufferID);

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_SheetWidth, m_SheetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	//	Keep it within [0, 1]
	m_SpriteWidth	= static_cast<float>(1.f / m_SpriteCountPerRow);
	m_SpriteHeight	= static_cast<float>(1.f / m_SpriteCountPerCol);
}


void SpriteSheet::ExtractSpriteRegionsToArray(
	std::vector<SpriteRegion>& OUT_spriteRegionVector
) {
	int SpriteCount = m_SpriteCountPerCol * m_SpriteCountPerRow;
	for (int i = 0; i < SpriteCount; i++) {
		
		float xSpriteCoord = i % m_SpriteCountPerRow;
		float ySpriteCoord = i / m_SpriteCountPerRow;

		OUT_spriteRegionVector.emplace_back(
			xSpriteCoord * m_SpriteWidth, ySpriteCoord * m_SpriteHeight,
			(xSpriteCoord + 1) * m_SpriteWidth,
			(ySpriteCoord + 1) * m_SpriteHeight,
			m_TextureBufferID,
			m_Shader
		);
	}
}

UVRegion::UVRegion(
	float _u0, float _v0,
	float _u1, float _v1
)
	: u0(_u0), v0(_v0),
	u1(_u1), v1(_v1)
{}


SpriteRegion::SpriteRegion(
	float _u0, float _v0,
	float _u1, float _v1,
	unsigned _textureID,
	Shader* _shader
)
	: Region(_u0, _v0, _u1 ,_v1),
	TextureID(_textureID),
	ShaderPtr(_shader)
{}

void SpriteSheet::DestroyGLTextureObject() { glDeleteTextures(1, &m_TextureBufferID); }
const std::string& SpriteSheet::GetName() const { return m_SheetName; }
SpriteSheet::SpriteSheet() {}