#include "shader.h"



Shader::Shader(
    const std::string& _locationShaderFile,
    const std::string& _shaderName
) 
    : m_ShaderName(_shaderName)
{
    ShaderProgramSources src = ParseShader(_locationShaderFile);
    m_ProgramID = CreateShader(src.VertexSource, src.FragmentSource);
}


const void Shader::UseShader() const {
    DEBUG_ASSERT(m_ProgramID != 0, "Using uninitialised shader.")
    glUseProgram(m_ProgramID);
}

void Shader::SetMat4(
    const char* _uniformName,
    const glm::mat4& _uniformValue
) const {
    int a = glGetUniformLocation(GetShaderId(), _uniformName);
    glUniformMatrix4fv(
        glGetUniformLocation(GetShaderId(), _uniformName),
        1,
        GL_FALSE,
        glm::value_ptr(_uniformValue)
    );
}

void Shader::SetFloat(
    const char* _uniformName,
    const float _uniformValue
) const {
    glUniform1f(
        glGetUniformLocation(GetShaderId(), _uniformName),
        _uniformValue
    );
}


const std::string& Shader::GetName() const { return m_ShaderName; }
const unsigned int Shader::GetShaderId() const { return m_ProgramID; }
