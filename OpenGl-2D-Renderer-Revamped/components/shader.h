#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


#include "../common/common.h"



#define GLCall(x) GLClearError();\
    x;\
    DEBUG_ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL ERR] (" << error << "): " << function << " " << file << ":" << line << '\n';
        return false;
    }
    return true;
}

struct ShaderProgramSources {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSources ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    DEBUG_ASSERT(stream.is_open(), std::string(filepath).insert(0, "Bad shader file at: "))

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;

    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos) {
                //set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                //set mode to frag
                type = ShaderType::FRAGMENT;
            }
        }
        else
            ss[(int)type] << line << "\n";
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));  //dynamic stack allocation
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << "\n";
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

/*
*   Shader class
*   Represents a named program object with several attached shaders to it.
*   This means a vertex and a fragment shader.
* 
*   For the purposes of creating this framework,
*   An OpenGL *program* object, which contains several attached *shader* objects
*   within it, will be called a *shader* for convenience.
*/

class Shader {

    unsigned int m_ProgramID = 0;

    std::string m_ShaderName;

    

public:

    Shader() {}

    Shader(
        const std::string& _locationShaderFile,
        const std::string& _shaderName
    );

    const unsigned int GetShaderId() const;

    const std::string& GetName() const;

    const void UseShader() const;
};

