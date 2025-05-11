#include "sprite_sheet.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../dependancies/stb_image.h"



glm::vec2 SpriteSheet::GetCalculatedSpriteOffsets(
	int _spriteIndex
) const {
	DEBUG_ASSERT(_spriteIndex >= 0 && _spriteIndex < m_SpriteCountPerCol * m_SpriteCountPerCol, "Sprite index out of bounds.");
	
	int xCoord = _spriteIndex % m_SpriteCountPerRow;
	int yCoord = _spriteIndex / m_SpriteCountPerRow;

	return { xCoord * m_SpriteUniformUVs.u1, yCoord * m_SpriteUniformUVs.v1 };
}

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
	m_SpriteUniformUVs.u0 = 0;	m_SpriteUniformUVs.v0 = 0;
	m_SpriteUniformUVs.u1 = static_cast<float>(1.f / m_SpriteCountPerRow);
	m_SpriteUniformUVs.v1 = static_cast<float>(1.f / m_SpriteCountPerCol);
}


void SpriteSheet::DestroyGLTextureObject() { glDeleteTextures(1, &m_TextureBufferID); }
const std::string& SpriteSheet::GetName() const { return m_SheetName; }
SpriteSheet::SpriteSheet() {}



UVRegion::UVRegion(
	float _u0, float _v0,
	float _u1, float _v1
)
	: u0(_u0), v0(_v0),
	u1(_u1), v1(_v1)
{}





bool UVRegion::operator==(const UVRegion& other) const {
	
	return fEqual(u0, other.u0)
		&& fEqual(v0, other.v0)
		&& fEqual(u1, other.u1)
		&& fEqual(v1, other.v1);
}

