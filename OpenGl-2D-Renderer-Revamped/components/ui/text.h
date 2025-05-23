#pragma once

#include "font.h"

/*
* Text is similar StrictBatch, but its texture data
* is in a special wrapper called Font and there are 
* a little more rules to rendering it.
*/

/*
TODO:	A staggering limitation has appeared, this Text
		object is basically a line that can't wrap around.

*/

struct TextOptions {
	float m_LineLength = 0.f;
	float m_UpperBound = 0.f;
	float m_LowerBound = 0.f;

	float m_ScaleFactor = 1.f;

	float m_CharacterHeight;
	float m_SpacingBetweenLines;

	TextOptions() {}

	TextOptions(
		float _charHeight,
		float _spacingBetweenLines
	):
		m_CharacterHeight(_charHeight),
		m_SpacingBetweenLines(_spacingBetweenLines)
	{}
};

class Text {

	std::u32string m_TextContent;
	std::vector<unsigned short> m_OffsetsFromFirstGlyph;
	std::vector<int> m_LineBreaks; // needs to be cast to int for uniform anyway
	std::vector<float> m_LineLengths;
	
	const Font* m_Font = nullptr;

	TextOptions m_TextOptions;


	//	Since appending and removing from the string will take place
	//	we need to track this for know if we can just buffer data
	//	or allocated memori is insufficient and we need more.
	size_t m_VBOlastReportedSize = 0;
	//	This buffer holds pairs of unsigned shorts for each instance
	unsigned int m_GlyphDataVBO = 0;


	float m_fRightWordWrapBound = 0.f;
	int m_MaximumCharsPerLine = 0;
	
public:

	Text();


	Text(
		const Font* _font,
		const std::u32string& _string,
		TextOptions _textOptions
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


	void SetWordWrapBound(
		float _rightBound
	);


	void SetMaximumCharactersPerLine(
		int _charsPerLine
	);


	void CalculateWordWraps();

private:	

	static unsigned int s_VAO;

public:		// globally available for rendering

	static void BindCommonVAO();
	static void UnbindCommonVAO();

public:

	const TextOptions& GetTextOptions() const { return m_TextOptions; }
	const std::vector<float>& GetLineLengthsArray() const { return m_LineLengths; }
	const std::vector<int>& GetLineBreakArray() const { return m_LineBreaks; }
	size_t GetLineBreakCount() const { return m_LineBreaks.size(); }
	int GetMaxCharsPerLine() const { return m_MaximumCharsPerLine; }
	float GetRightWordWrapBound() const { return m_TextOptions.m_LineLength; }
	//float GetScaleFactor() const { return m_ScaleFactor; }
	unsigned int GetGlyphDataVBO() const { return m_GlyphDataVBO; }
	const std::u32string& GetTextString() const { return m_TextContent; }
	const Font* GetFont() const { return m_Font; }
	size_t GetCharCount() const { return m_TextContent.size(); }

private:

	unsigned short GetTotalOffsetForCharAt(
		int _charIndex
	);


	void UpdateGlyphOffsets();


	void UpdateVBO();


	void GenerateBuffers();


	size_t GetLastVBOsize() const { return m_VBOlastReportedSize; }

};

