#pragma once

#include "label.h"

class Input : public WidgetCompositionInterface {

	Label* m_TextContent = nullptr;

public:

	Input(
		glm::vec2 offsetRelToParent,
		glm::vec2 dimensions,
		const Text& text,
		const PaneSkin* bgSkin
	) :
		WidgetCompositionInterface(
			offsetRelToParent,
			dimensions,
			bgSkin,
			true
		)
	{
		auto label = std::make_unique<Label>(text, glm::vec2(5.f, 5.f), dimensions - glm::vec2(5.f, 5.f));

		m_TextContent = label.get();

		AddChild(std::move(label));
	}

	void OnClick(EventEmitter* ctx, Window* owningWindow) {
		Event self;
		self.type = EventType::SET_FOCUS_ON_INPUT;
		self.setInputFocusEvent.targetInputPointer = this;
		ctx->PushEvent(self);
	}

	void AppendCharacter(
		char32_t ch
	);

	void AppendString(
		const std::u32string& str
	);

	//	Backspace
	void RemoveLastCharacter();

	//	Ctrl Backspace
	void ClearTillLastSpace();
	
	//	Ctrl Delete
	void ClearTillNextSpace();

	void ClearText();

	const std::u32string* GetValue() const { return &m_TextContent->GetText()->GetTextString(); }

	virtual void SendOwnRenderData(
		Batch* uiBatch,
		glm::vec2 absoluteCurrentWidgetOrigin,
		float z
	) const {}

private:

	Text* GetText() { return m_TextContent->GetText(); }

};