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


#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>

#include "../common/common.h"

#include "shader.h"


struct alignas(float) UVRegion {

	UVRegion() {}
	//	  x		  y
	float u0 = 0, v0 = 0;	
	float u1 = 0, v1 = 0;

	UVRegion(
		float _u0, float _v0,
		float _u1, float _v1
	);

	void* GetData() { return this; }
	bool operator==(const UVRegion& other) const;

};


enum class SpriteSheetType {
	NotInitialised = 0,
	IndexBased,
	DirectUV,

	ErrorLoading
};


struct TextureParamsDataStruct {
	int S_WrapMode = -1;
	int T_WrapMode = -1;
};


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

private:

	SpriteSheetType m_Type = SpriteSheetType::NotInitialised;
	TextureParamsDataStruct m_TexParams;

private:

	std::vector<UVRegion> m_UVregionsFromConfigFile;
	std::vector<std::string> m_UVregionNamesFromConfigFile;

public:

	glm::vec2 GetCalculatedSpriteOffsets(
		int _spriteIndex
	) const;

public:

	/*
		If loading a sheet by config file, values for
		sprite counts per row and column will be ignored
	*/

	SpriteSheet(
		const std::string& _locationOfImageOrConfigFile,
		const std::string& _sheetName,
		const Shader* _preferredShader,
		int _spritesPerRow_IGNORED_IF_LOADING_CONFIG_FILE,
		int _spritesPerCol_IGNORED_IF_LOADING_CONFIG_FILE
	);


	SpriteSheet();


	unsigned short GetSpriteIndexByName(
		const char* _spriteName
	) const;


	//	!!!		ABANDONED	  !!!
	void TransformIndicesToUVRegionArray(
		const int* _indexArray,
		const int _indexArraySize,
		std::vector<UVRegion>& OUT_uvRegionArray,
		std::vector<float>* OUT_vertexArray = nullptr
	) const;

private:	//	Private getters 

	//const UVRegion* GetUVRegionArray() const { return m_UVregionsFromConfigFile.data(); }

private:

	const int c_StandardImageLoadingMethodReturnCode	= 1;
	const int c_ConfigFileLoadingMethodReturnCode		= 2;
	const int c_ErrorInLoadingMethodReturnCode			= -1;
		

	int DetermineLoadingMethodFromGivenPath(
		const std::string& _pathFromConstructor
	);


	void ConfigurationPairLoadingMethod(
		const char* _pathToConfigFile
	);


	void StandardImageLoadingMethod(
		const char* _pathFromConstructor,
		int _spritesPerRow,
		int _spritesPerCol
	);


	void LoadImageInTexture(
		const char* _pathToFile
	);


	void InterpretTextureParametersString(
		const std::string& _texParamsLine
	);


	void SetTextureParametersToGL();

public:

	const UVRegion* GetUVRegionByName ( const char* _assetName ) const;
	SpriteSheetType GetType() const { return m_Type; }
	glm::vec2 GetSpriteDimensions() const;
	const UVRegion& GetSheetSpriteUVregion() const { return m_SpriteUniformUVs; }
	const std::string& GetName() const;
	const Shader* GetShader() const { return m_Shader; }
	int GetSheetRowSpriteCount() const { return m_SpriteCountPerRow; }
	unsigned int GetTextureBufferID() const { return m_TextureBufferID; }
	const TextureParamsDataStruct& GetTexParams() const { return m_TexParams; }

	const std::vector<UVRegion>& GetUVRegionArray() const { return m_UVregionsFromConfigFile; }

	const int GetContainedSpriteCount() const { return static_cast<int>(m_UVregionsFromConfigFile.size()); }

	const int GetSpriteSheetWidth() const { return m_SheetWidth; }
	const int GetSpriteSheetHeight() const { return m_SheetHeight; }

	void DestroyGLTextureObject();

};


