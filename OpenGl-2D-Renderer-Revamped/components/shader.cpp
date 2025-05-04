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
    DEBUG_ASSERT(m_ProgramID != 0, std::string(m_ShaderName).append(" <> Unitiliased shader used.\n").c_str())
    glUseProgram(m_ProgramID);
}


const std::string& Shader::GetName() const { return m_ShaderName; }
const unsigned int Shader::GetShaderId() const { return m_ProgramID; }
