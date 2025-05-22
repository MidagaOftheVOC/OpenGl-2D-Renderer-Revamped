#include "shader.h"




struct ShaderProgramSources {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSources ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    DEBUG_ASSERT(stream.is_open(), "Failed to open shader file: %s", filepath.c_str())

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


Shader::Shader(
    const std::string& _locationShaderFile,
    const std::string& _shaderName
) 
    : m_ShaderName(_shaderName)
{
    ShaderProgramSources src = ParseShader(_locationShaderFile);
    m_ProgramID = CreateShader(src.VertexSource, src.FragmentSource);
    InitialiseUniformLocationMap();
}


const void Shader::UseShader() const {
    DEBUG_ASSERT(m_ProgramID != 0, "Using uninitialised shader!")
    glUseProgram(m_ProgramID);
}


void Shader::InitialiseUniformLocationMap() {

    unsigned int Program = GetShaderId();
    int LocationCount;
    glGetProgramiv(Program, GL_ACTIVE_UNIFORMS, &LocationCount);

    for(int i = 0; i < LocationCount; i++){
        char UniName[100] = { 0 };

        int Length;
        int Size;
        GLenum Type;

        glGetActiveUniform(Program, i, sizeof(UniName) - 1, &Length, &Size, &Type, UniName);

        int Location = glGetUniformLocation(Program, UniName);
        DEBUG_ASSERT(Location != -1, "Uniform location is -1 in shader \"%s\".", GetName().c_str());

        m_UniformLocationMap[UniName] = { Location , Type };
    }
}


void Shader::ApplyUniforms(
    const UniformDataVector* _uniformArray
) const {
    if (!_uniformArray) return;
    for (const auto& Uniform : *_uniformArray) {

        auto Iterator = m_UniformLocationMap.find(Uniform.name);

        if (Iterator == m_UniformLocationMap.end()) {
            DEBUG_LOG("Non-existant uniform [%s] within shader [%s]", Uniform.name.c_str(), GetName().c_str());
        }
        else {

            const int Location = Iterator->second.location;
            const GLenum Type = Iterator->second.type;


            std::visit([this, &Location, &Type](auto&& value) {

                using T = std::decay_t<decltype(value)>;


                if constexpr (std::is_same_v<T, glm::mat4>) {
                    glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(value));
                    return;
                } 
                else if constexpr (std::is_same_v <T, float>) {
                    glUniform1f(Location, value);
                    return;
                }
                else if constexpr (std::is_same_v<T, int>) {
                    glUniform1i(Location, value);
                    return;
                } else 

#define IF_TYPE(T_, ToType_, function_)\
if constexpr (std::is_same_v<T_, ToType_>) {\
function_; return;}

                IF_TYPE(T, glm::vec2, glUniform2f(Location, value.x, value.y)) else
                IF_TYPE(T, glm::vec3, glUniform3f(Location, value.x, value.y, value.z))


                }, Uniform.value);
        }
    }
}


int Shader::GetUniformLocation(
    const char* _uniformName
) const {
    auto Iterator = m_UniformLocationMap.find(_uniformName);
    
    if (Iterator != m_UniformLocationMap.end()) {
        return Iterator->second.location;
    }

    DEBUG_WARN(1, "Shader [%s] can't find uniform with name [%s].", GetName().c_str(), _uniformName);
    return -1;
}


void Shader::SetMat4(
    const char* _uniformName,
    const glm::mat4& _uniformValue
) const {
    glUniformMatrix4fv(
        GetUniformLocation(_uniformName),
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
        GetUniformLocation(_uniformName),
        _uniformValue
    );
}

void Shader::SetInt(
    const char* _uniformName,
    const int _uniformValue
) const {
    glUniform1i(
        GetUniformLocation(_uniformName),
        _uniformValue
    );
}

void Shader::SetVec2(
    const char* _uniformName,
    const glm::vec2 _uniformValue
) const {
    glUniform2f(
        GetUniformLocation(_uniformName),
        _uniformValue.x,
        _uniformValue.y
    );
}


const std::string& Shader::GetName() const { return m_ShaderName; }
const unsigned int Shader::GetShaderId() const { return m_ProgramID; }


