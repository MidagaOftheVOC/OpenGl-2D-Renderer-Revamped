#pragma once

#include <vector>

class Input;

enum class EventType {
	CLOSE_WINDOW,
	SET_FOCUS_ON_INPUT
};

struct CloseWindowEvent {
	unsigned int targetWindowID;
};

struct SetInputFocusEvent {
	Input* targetInputPointer;
};

struct Event {
	EventType type;

	union {
		CloseWindowEvent closeWindowEvent;
		SetInputFocusEvent setInputFocusEvent;
	};
};

class EventEmitter {

	std::vector<Event> m_Events;

public:

	EventEmitter() {}

	void PushEvent(Event event) {
		m_Events.emplace_back(event);
	}

	std::vector<Event>& GetEvents() { return m_Events; }

};