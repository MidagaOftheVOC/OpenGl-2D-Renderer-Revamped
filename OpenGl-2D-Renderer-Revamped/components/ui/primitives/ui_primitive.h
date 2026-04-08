#pragma once

#include <vector>
#include <memory> // added to ensure std::unique_ptr is available

#include "../../common/common.h"
#include "../../common/standard_quad.h"

#include "../../batch_types/batch_instance_primitives.h"

#include "../text.h"

/*

Abstract class for all graphical primitives.

*/

struct WidgetWindowData {
	glm::vec2 WindowDimensions;
};

struct SubRect {
	std::vector<float> OUT_batchPairsOfXYdimensions;
	std::vector<float> OUT_batchPairsOfXYpositions;
	std::vector<SpriteInformation>& OUT_batchSpriteInformation;
	float _xOffset;
	float _yOffset;
};

struct TextWithZLayer {
	TextWithZLayer(
		const Text* _text,
		float _x,
		float _y,
		float _zLayer
	) :
		text(_text),
		xLayer(_x),
		yLayer(_y),
		zLayer(_zLayer)
	{
	}
	const float xLayer = 0.f;
	const float yLayer = 0.f;
	const float zLayer = 0.f;
	const Text* text = nullptr;
};

struct UI_Primitive {
protected:

	bool m_Clickable = true;

protected:

	glm::vec2 m_Dimensions = { 0.f, 0.f };

	//	Drawcalls will be made to the upper left corner of the window object
	//	therefore primitives require offset from that point.
	glm::vec2 m_PositionRelativeToWindow = { 0.f, 0.f };

	std::vector<std::unique_ptr<UI_Primitive>> m_WidgetComposition;

public:

	virtual ~UI_Primitive() = default; // ADDED: virtual dtor for safe polymorphic deletion

	UI_Primitive() {}

	UI_Primitive(
		glm::vec2 _relativeToWindow
	);

	UI_Primitive(
		glm::vec2 _dimensions,
		glm::vec2 _relativeToWindow
	);

private:

	virtual void OnDimensionChange() {}	//<<< why is this here??
	virtual void OnClick() {}
	virtual void OnHover() {}
	virtual void OnFocus() {}

public:

	bool ContainsMouseClick(
		glm::vec2 point,
		glm::vec2 windowOrigin
	) const;

public:

	const glm::vec2 GetPositionRelativeToWindow() const { return m_PositionRelativeToWindow; }
	const glm::vec2 GetDimensions() const { return m_Dimensions; }

	void SetDimensions(const glm::vec2& _dimensions);
	void SetPositionRelativeToWindow(const glm::vec2& _posRelativeToWindow);
	bool IsClickable() const { return m_Clickable; }

	virtual void PostAttachment(WidgetWindowData _data) = 0;

	//	Develop this idea. Also, needs centralised way of acquiring resources for each widget, they need to hold
	//	only ptrs to rendering data and append it when commanded to.
	virtual void AppendWidgetRenderDataToArray(std::vector<float> &OUT_rects, std::vector<TextWithZLayer> &OUT_texts, float zLayer) = 0;

	const UI_Primitive* DetermineIfClicked(
		const glm::vec2 parentOrigin,
		const glm::vec2 mouseCoords
	) const;

};