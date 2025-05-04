#pragma once


/*
* Regular Sprite sheet where each sprite is next to each other
* and all have uniform dimensions
*/

#include <string>
#include <vector>

#include "drawable.h"



#define GLEW_STATIC
#include "../dependancies/GL/glew.h"
#include "../dependancies/GLFW/glfw3.h"

#include "../dependancies/stb_image.h"

#include "shader.h"

class SpriteSheet {

	int m_SheetWidth = -1;
	int m_SheetHeight = -1;

	//	These will be calculated at the creation of each object
	float m_SpriteWidth = -1;
	float m_SpriteHeight = -1;

	int m_SpriteCountPerRow = -1;	//	pieces per row
	int m_SpriteCountPerCol = -1;	//	pieces per col

	//	will be used for easier, but slower, search in Renderer2D
	std::string m_SheetName;

	unsigned int m_TextureBufferID = 0x7FFFFFFF;
	const Shader* m_Shader = nullptr;

public:

	SpriteSheet(
		const std::string& _locationRawImage,
		const std::string& _sheetName,
		const Shader* _preferredShader,
		int _spritesPerRow,
		int _spritesPerCol
	);


	SpriteSheet();


	//	ABANDONED
	//	Actions to be taken after supplying basic information like:
	//	Image location and sprite counts
	void AfterCreation();


	const std::string& GetName() const;


	//	TODO: idea for operator
	//	Drawable operator[](int index)
	
	void ExtractSpriteRegionsToArray(
		std::vector<SpriteRegion>& OUT_spriteRegionVector
	);


	void DestroyGLTextureObject();

};


struct UVRegion {
	float u0, v0;
	float u1, v1;

	UVRegion(
		float _u0, float _v0,
		float _u1, float _v1
	);
};

struct SpriteRegion {
	UVRegion Region;
	unsigned int TextureID = 0;
	const Shader* ShaderPtr = nullptr;

	SpriteRegion(
		float _u0, float _v0,
		float _u1, float _v1,
		unsigned _textureID,
		Shader* _shader
	);
};

