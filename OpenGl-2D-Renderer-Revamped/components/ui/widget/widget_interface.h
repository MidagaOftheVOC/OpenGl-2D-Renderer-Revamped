#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "../text.h"
#include "../../batch_types/base_batch.h"

struct PaneSkin {
	int cornerLengthPx = 0;
	std::string name;
	SpriteInstance instanceArray[9];
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

//	The 'Pane'/'Rect' concept will exist not as a widget, but the interface will
//	hold data about the rectanlge AND which skin to use.
//	

class WidgetCompositionInterface {

	std::vector<std::unique_ptr<WidgetCompositionInterface>> m_WidgetComposition;

private:

	glm::vec2 m_PositionRelToParent = glm::vec2(0.f, 0.f);
	glm::vec2 m_Dimensions = glm::vec2(0.f, 0.f);

	bool m_IsHitTestable = true;	//<<< Set to true, very advanced concept
	bool m_IsInteractable = false;

	const PaneSkin* m_BackgroundSkin = nullptr;

private:

	std::function<void()> m_OnClickFn;

public:

	void OnClick() {
		if (m_OnClickFn) {
			m_OnClickFn();
		}
	}

public:

	WidgetCompositionInterface() {}

	WidgetCompositionInterface(
		const PaneSkin* background,
		bool isInteractable = true
	) :
		m_IsInteractable(isInteractable),
		m_BackgroundSkin(background)
	{}

	WidgetCompositionInterface(WidgetCompositionInterface& other) = delete;
	WidgetCompositionInterface& operator=(const WidgetCompositionInterface& other) = delete;

public:

	void AddChild(std::unique_ptr<WidgetCompositionInterface> widget) {
		m_WidgetComposition.push_back(std::move(widget));
	}

	//	Note: design is smooth except here
	void RenderWidgetTree(
		Batch* uiBatch,
		std::vector<TextWithZLayer>& texts,
		glm::vec2 absoluteParentOrigin,
		float baseZLayer,
		float zSubstep,
		float zSubposition
	);

	WidgetCompositionInterface* DetectClick(
		glm::vec2 absoluteParentOrigin,
		glm::vec2 mousePosition
	);

private:

	virtual void SendOwnRenderData(
		Batch* uiBatch,
		std::vector<TextWithZLayer>& texts,
		glm::vec2 absolutePosition,
		float z
	) = 0;

	void SendStandardisedRenderData(
		Batch* uiBatch
	);

	glm::vec2 GetAbsolutePosition(
		glm::vec2 parentOrigin
	) const;

public:

	void SetOnClick(
		std::function<void()> fn
	);

public:

	const std::vector<std::unique_ptr<WidgetCompositionInterface>>& GetWidgets() { return m_WidgetComposition; }

};