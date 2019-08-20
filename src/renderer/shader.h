#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Jam3D {

// Interface for OpenGL shader program. 
class Shader
{
private:
    struct ShaderSources
    {
        std::string vertexSource;
        std::string geometrySource;
        std::string fragmentSource;

        ShaderSources(std::string vSrc, std::string gSrc, std::string fSrc) {vertexSource = vSrc; geometrySource = gSrc; fragmentSource = fSrc; }
        ShaderSources(const ShaderSources &orig) {vertexSource = orig.vertexSource; geometrySource = orig.geometrySource; fragmentSource = orig.fragmentSource; }
    };

    std::string m_FilePath;
    unsigned int m_RendererID;
    unsigned int m_Type;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    enum : unsigned int {
        VERTEX_FRAGMENT, VERTEX_GEOMETRY_FRAGMENT
    };

    Shader(const std::string& fp, unsigned int type);

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

    int GetUniformLocation(const std::string& name);

private:
    unsigned int CreateShader(ShaderSources src);
    int CompileShader(unsigned int type, const std::string& source);
    ShaderSources ParseShader(const std::string& fp);
};

}