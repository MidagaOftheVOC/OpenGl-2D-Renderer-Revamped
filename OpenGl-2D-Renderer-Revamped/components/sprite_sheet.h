#pragma once


/*
* Regular Sprite sheet where each sprite is next to each other
* and all have uniform dimensions
* 
*	SpriteSheet		- an image subdivided in N x M uniform regions
*	Sprite index	- subregions in the sprite sheet are indexed 
*					  left to right, top to bottom, starting from 0
*	
* 
*/

#include <string>
#include <vector>

#include "../common/common.h"



#include "shader.h"


struct UVRegion {

	UVRegion() {}
	//	  x		  y
	float u0 = 0, v0 = 0;	
	float u1 = 0, v1 = 0;

	UVRegion(
		float _u0, float _v0,
		float _u1, float _v1
	);

	bool operator==(const UVRegion& other) const;

};
//
//struct SpriteRegion {
//	UVRegion Region;
//	unsigned int TextureID = 0;
//	const Shader* ShaderPtr = nullptr;
//
//
//	SpriteRegion(
//		float _u0, float _v0,
//		float _u1, float _v1,
//		unsigned _textureID,
//		const Shader* _shader
//	);
//};
//



class SpriteSheet {

	int m_SheetWidth = -1;
	int m_SheetHeight = -1;

	//	These will be calculated at the creation of each object
	UVRegion m_SpriteUniformUVs;

	int m_SpriteCountPerRow = -1;	//	pieces per row
	int m_SpriteCountPerCol = -1;	//	pieces per col

	//	will be used for easier, but slower, search in Renderer2D
	std::string m_SheetName;

	unsigned int m_TextureBufferID = 0x7FFFFFFF;
	const Shader* m_Shader = nullptr;

public:


	glm::vec2 GetCalculatedSpriteOffsets(
		int _spriteIndex
	) const;

	const UVRegion& GetSheetSpriteUVregion() const { return m_SpriteUniformUVs; }


public:

	SpriteSheet(
		const std::string& _locationRawImage,
		const std::string& _sheetName,
		const Shader* _preferredShader,
		int _spritesPerRow,
		int _spritesPerCol
	);


	SpriteSheet();

	const std::string& GetName() const;
	const Shader* GetShader() const { return m_Shader; }
	unsigned int GetTextureBufferID() const { return m_TextureBufferID; }

	void DestroyGLTextureObject();
};


