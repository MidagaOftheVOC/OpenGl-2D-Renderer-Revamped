
#include "common.h"

template <typename t>
vec2<t> vec2<t>::operator+(const vec2<t>& other) {
	return { x + other.x, y + other.y };
}


template <typename t>
vec2<t> vec2<t>::operator-(const vec2<t>& other) {
	return { x - other.x, y - other.y };
}


template <typename t>
float vec2<t>::GetLength() {
	return static_cast<float>(sqrt(x * x + y * y));
}

template <typename t>
void vec2<t>::SetAngle(float radian) {
	float vecLength = GetLength();

	t xNew, yNew;
	float length = GetLength();

	xNew = x * cos(radian) * length;
	yNew = y * sin(radian) * length;

	x = xNew;
	y = yNew;
}


bool fEqual(
	float _val1,
	float _val2) 
{
	return 
		( abs(_val1 - _val2) < FLOAT_COMPARE_TOLERANCE ) ? true : false;
}


void CheckGLErrors(const char* context) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		const char* errorStr = "Unknown error";
		switch (err) {
		case GL_INVALID_ENUM:      errorStr = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE:     errorStr = "GL_INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: errorStr = "GL_INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:    errorStr = "GL_STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:   errorStr = "GL_STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:     errorStr = "GL_OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: errorStr = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cerr << "[OpenGL Error] (" << errorStr << ") in context: " << context << std::endl;
	}
}