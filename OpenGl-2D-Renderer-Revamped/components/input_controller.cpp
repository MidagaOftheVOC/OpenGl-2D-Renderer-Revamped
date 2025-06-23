#include "input_controller.h"


InputController::InputController(
	GLFWwindow* _mainWindowHandle
) :
	m_MainWinHandle(_mainWindowHandle)
{}


bool InputController::AccessBitmask(
	int _index,
	const std::bitset<MAXIMUM_BITSET_SIZE> &_bitmask
) const {
	return _bitmask[_index];
}


bool InputController::AccessPreviousKeystateBitmask(
	int _index
) const {
	return AccessBitmask(_index, m_PreviousKeystateBitmask);
}


bool InputController::AccessRecentKeystateBitmask(
	int _index
) const {
	return AccessBitmask(_index, m_RecentKeystateBitmask);
}


void InputController::CaptureKeystates() {

	DEBUG_ASSERT(m_MainWinHandle != nullptr, "InputController used when Window handle is null.");

	m_PreviousKeystateBitmask = m_RecentKeystateBitmask;
	
	for (int i = 32; i < MAXIMUM_TRACKED_KEYS_INDEX; i++) {
		if(m_TrackedKeystatesBitmask.test(i))
			m_RecentKeystateBitmask[i] = glfwGetKey(m_MainWinHandle, i);
	}

	//	Mouse coords

	double xMousePosition, yMousePosition;
	glfwGetCursorPos(m_MainWinHandle, &xMousePosition, &yMousePosition);

	m_xMouseCoord = static_cast<float>(xMousePosition);
	m_yMouseCoord = static_cast<float>(yMousePosition);
}


bool InputController::IsPressed(
	GLuint _openglKeyCode
) const {
	DEBUG_ASSERT(_openglKeyCode >= 32 && _openglKeyCode <= MAXIMUM_TRACKED_KEYS_INDEX,
		"Attempt to check out-of-bounds keystate of keycode [%d].", _openglKeyCode);
	return
		AccessRecentKeystateBitmask(_openglKeyCode) &&
		!AccessPreviousKeystateBitmask(_openglKeyCode);
}


bool InputController::IsHeld (
	GLuint _openglKeyCode
) const {
	DEBUG_ASSERT(_openglKeyCode >= 32 && _openglKeyCode <= MAXIMUM_TRACKED_KEYS_INDEX,
		"Attempt to check out-of-bounds keystate of keycode [%d].", _openglKeyCode);
	return
		AccessRecentKeystateBitmask(_openglKeyCode) &&
		AccessPreviousKeystateBitmask(_openglKeyCode);
}


bool InputController::IsReleased (
	GLuint _openglKeyCode
) const {
	DEBUG_ASSERT(_openglKeyCode >= 32 && _openglKeyCode <= MAXIMUM_TRACKED_KEYS_INDEX,
		"Attempt to check out-of-bounds keystate of keycode [%d].", _openglKeyCode);
	return
		!AccessRecentKeystateBitmask(_openglKeyCode) &&
		AccessPreviousKeystateBitmask(_openglKeyCode);
}


void InputController::SetTrackedKeystatesBitmask(
	TrackingBit _trackingBitmask
) {
	m_TrackedKeystatesBitmask.reset();

	if (_trackingBitmask & c_LetterTrackBit) {
		AddIndicesToTrackedKeystatesBitmask(
			c_LetterKeystateIndices,
			sizeof(c_LetterKeystateIndices) / sizeof(int)
		);
	}

	if (_trackingBitmask & c_DigitTrackBit) {
		AddIndicesToTrackedKeystatesBitmask(
			c_DigitKeystateIndices,
			sizeof(c_DigitKeystateIndices) / sizeof(int)
		);
	}

	if (_trackingBitmask & c_ArrowTrackBit) {
		AddIndicesToTrackedKeystatesBitmask(
			c_ArrowKeystateIndices,
			sizeof(c_ArrowKeystateIndices) / sizeof(int)
		);
	}

	if (_trackingBitmask & c_SpecialSymbolTrackBit) {
		AddIndicesToTrackedKeystatesBitmask(
			c_SpecialSymbolKeystateIndices,
			sizeof(c_SpecialSymbolKeystateIndices) / sizeof(int)
		);
	}

	if (_trackingBitmask & c_FunctionTrackBit) {
		AddIndicesToTrackedKeystatesBitmask(
			c_FunctionKeystateIndices,
			sizeof(c_FunctionKeystateIndices) / sizeof(int)
		);
	}

	if (_trackingBitmask & c_SpecialTrackBit ) {
		AddIndicesToTrackedKeystatesBitmask(
			c_SpecialKeystateIndices,
			sizeof(c_SpecialKeystateIndices) / sizeof(int)
		);
	}
}


void InputController::AddIndicesToTrackedKeystatesBitmask(
	const int* _indexArray,
	int _indexElementCount
) {
	for (int i = 0; i < _indexElementCount; i++) {
		m_TrackedKeystatesBitmask.set( _indexArray[i] );
	}
}


void InputController::GetMousePosition(
	float& OUT_xMouseCoord,
	float& OUT_yMouseCoord
) const {
	OUT_xMouseCoord = m_xMouseCoord;
	OUT_yMouseCoord = m_yMouseCoord;
}
