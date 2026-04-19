#include "input.h"

void Input::AppendCharacter(
	char32_t ch
) {
	GetText()->AppendCharacter(ch);
}

void Input::AppendString(
	const std::u32string& str
) {
	auto* textValue = GetText();
	for (auto ch : str) {
		textValue->AppendCharacter(ch);
	}
}

//	Backspace
void Input::RemoveLastCharacter() {
	GetText()->RemoveLastCharacter();
}

//	Ctrl Backspace
void Input::ClearTillLastSpace() {

}

//	Ctrl Delete
void Input::ClearTillNextSpace() {

}

void Input::ClearText() {

}