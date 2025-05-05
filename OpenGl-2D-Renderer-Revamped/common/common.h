#pragma once

// -- PROJECT SETTINGS -- //

#define DEBUG__ALLOW_ASSERTS
#define GLEW_STATIC



// -- END PROJECT SETTINGS -- //



#include <cmath>
#include <cstdio>



#include "../dependancies/GL/glew.h"
#include "../dependancies/GLFW/glfw3.h"

#include "../dependancies/glm/glm.hpp"
#include "../dependancies/glm/gtc/matrix_transform.hpp"
#include "../dependancies/glm/gtc/type_ptr.hpp"



#define FLOAT_COMPARE_TOLERANCE 0.0001f


#ifdef DEBUG__ALLOW_ASSERTS

#define DEBUG_ASSERT(x_, msg_, ...) \
    if (!(x_)) { \
        fprintf(stderr, "%s : %d ASSERT FAILED: " msg_ "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        __debugbreak(); \
    }

#define DEBUG_WARN(x_, msg_, ...) \
    if (!(x_)) { \
        fprintf(stderr, "%s : %d WARNING: " msg_ "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    }

#define DEBUG_LOG(msg_, ...) \
    fprintf(stderr, "LOG: " msg_ "\n", ##__VA_ARGS__)

#else

#define DEBUG_ASSRT(x_, ASSERTS_ARE_DISABLED) 
#define DEBUG_ASSRT(x_, ASSERTS_ARE_DISABLED)
#define DEBUG_LOG(msg_)

#endif




struct StandardQuad {

	unsigned int m_VAO = 0;

	unsigned int m_VextexBuffer = 0;
	unsigned int m_TexCoordBuffer = 0;
	unsigned int m_IndexBuffer = 0;	

	void Init();
	void Bind();
	void Unbind();

};

extern StandardQuad g_StandardQuad;





template<typename t>
struct vec2 {

	t x = 0;
	t y = 0;

	vec2 operator+(const vec2& other);
	vec2 operator-(const vec2& other);

	float GetLength();

	//	Positive degrees is anti-clockwise
	void SetAngle(float radian);
};



bool fEqual(float _val1, float _val2);

