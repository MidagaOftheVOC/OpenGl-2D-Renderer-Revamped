
#include "common.h"

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
		__debugbreak();
	}
}