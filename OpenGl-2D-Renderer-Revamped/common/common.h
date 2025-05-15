#pragma once

// -- PROJECT SETTINGS -- //

#define DEBUG__CODE            // use for debug-build-only code snippets
#define DEBUG__ALLOW_ASSERTS
#define GLEW_STATIC



// -- END PROJECT SETTINGS -- //



#include <cmath>
#include <cstdio>
#include <iostream>


#include "../dependancies/GL/glew.h"
#include "../dependancies/GLFW/glfw3.h"

#include "../dependancies/glm/glm.hpp"
#include "../dependancies/glm/gtc/matrix_transform.hpp"
#include "../dependancies/glm/gtc/type_ptr.hpp"



#define FLOAT_COMPARE_TOLERANCE 0.0001f


#ifdef DEBUG__ALLOW_ASSERTS

#define DEBUG_ASSERT(x_, msg_, ...) \
    if (!(x_)) { \
        fprintf(stderr, "%s : %d\nASSERT FAILED: " msg_ "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        __debugbreak(); \
    }

#define DEBUG_WARN(x_, msg_, ...) \
    if (!(x_)) { \
        fprintf(stderr, "%s : %d\nWARNING: " msg_ "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    }

#define DEBUG_LOG(msg_, ...) \
    fprintf(stderr, "LOG: " msg_ "\n", ##__VA_ARGS__)

#else

#define DEBUG_ASSRT(x_, ASSERTS_ARE_DISABLED) 
#define DEBUG_ASSRT(x_, ASSERTS_ARE_DISABLED)
#define DEBUG_LOG(msg_)

#endif


void CheckGLErrors(const char* context = "OpenGL");

bool fEqual(float _val1, float _val2);


struct GL_DiagnosticFunctions {

    //static void Shader_



};

