#pragma once

// -- PROJECT SETTINGS -- //

#define GLEW_STATIC


#define DEBUG__CODE            // use for debug-build-only code snippets
#define DEBUG__ALLOW_ASSERTS

// -- END PROJECT SETTINGS -- //



#include <cmath>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <random>


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


struct GLdiagnostics {

    static bool IsVertexBuffer(
        unsigned int _bufferName,
        bool true_if_printing_result = false
    ) {

        bool IsBuffer = glIsBuffer(_bufferName) == GL_TRUE;
        
        if (true_if_printing_result) {

            if (IsBuffer) {
                fprintf(stdout, "[%u] is a valid buffer.\n", _bufferName);
            }
            else {
                fprintf(stdout, "[%u] is NOT a valid buffer.\n", _bufferName);
            }
        }

        return IsBuffer;
    }


    static bool IsVertexArray(
        unsigned int _vaoName,
        bool true_if_printing_result = false
    ) {

        bool IsVAO = glIsVertexArray(_vaoName) == GL_TRUE;

        if (true_if_printing_result) {

            if (IsVAO) {
                fprintf(stdout, "[%u] is a valid VAO.\n", _vaoName);
            }
            else {
                fprintf(stdout, "[%u] is NOT a valid VAO.\n", _vaoName);
            }
        }

        return IsVAO;
    }

};


struct Random {

    static std::mt19937& engine() {
        static std::mt19937 eng{ std::random_device{}() };
        return eng;
    }


    static int IntRange(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(engine());
    }


    static uint32_t UInt32Range(uint32_t min, uint32_t max) {
        std::uniform_int_distribution<uint32_t> dist(min, max);
        return dist(engine());
    }

};


struct Profiler {
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    static inline TimePoint s_Start;
    static inline TimePoint s_End;


    static void Start() {
        s_Start = Clock::now();
    }


    static float Stop() {
        s_End = Clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(s_End - s_Start).count();
        float millis = static_cast<float>(duration);
        return millis;
    }


    static long long ElapsedMicroseconds() {
        return std::chrono::duration_cast<std::chrono::microseconds>(s_End - s_Start).count();
    }


    static double ElapsedMilliseconds() {
        return std::chrono::duration<double, std::milli>(s_End - s_Start).count();
    }
};

