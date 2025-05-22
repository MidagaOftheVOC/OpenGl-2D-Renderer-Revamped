#pragma once

#include "font.h"

/*
* Text is similar StrictBatch, but its texture data
* is in a special wrapper called Font and there are 
* a little more rules to rendering it.
*/

class Text {

	std::u32string m_TextContent;
	std::vector<unsigned short> m_OffsetsFromFirstGlyph;
	
	const Font* m_Font = nullptr;


	float m_ScaleFactor = 1.f;


	//	Since appending and removing from the string will take place
	//	we need to track this for know if we can just buffer data
	//	or allocated memori is insufficient and we need more.
	int m_VBOlastReportedSize = 0;
	//	This buffer holds pairs of unsigned shorts for each instance
	unsigned int m_GlyphDataVBO = 0;

	static unsigned int s_VAO;
	
public:

	Text();


	Text(
		const Font* _font,
		const std::u32string& _string
	);


	//	This could be called from any random Text instance but it has to 
	//	be called only once in total. The idea is to have the common VAO
	//	bind `some` unique buffer, but at rendering it can be replaced
	//	with per-instance buffers
	static void InitialiseCommonVAO();


	void UpdateTextValue(
		std::u32string&& _stringToMove
	);


	void UpdateTextValue(
		const std::u32string& _stringToCopy
	);


	void AppendCharacter(
		char32_t _char
	);


	void RemoveCharacter(
		char32_t _char
	);


	//	This should only be called after this class' VAO is bound
	void BindUniqueBuffers() const;

public:	// globally available

	static void BindCommonVAO();
	static void UnbindCommonVAO();

public:

	unsigned int GetGlyphDataVBO() const { return m_GlyphDataVBO; }
	float GetScaleFactor() const { return m_ScaleFactor; }
	const std::u32string& GetTextString() const { return m_TextContent; }
	const Font* GetFont() const { return m_Font; }
	size_t GetCharCount() const { return m_TextContent.size(); }

private:

	void UpdateGlyphOffsets();


	void UpdateVBO();


	void GenerateBuffers();


	int GetLastVBOsize() const { return m_VBOlastReportedSize; }

};

