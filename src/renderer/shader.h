#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

// Interface for OpenGL shader program. 
class Shader
{
private:
    struct ShaderSources
    {
        std::string vertexSource;
        std::string fragmentSource;

        ShaderSources(std::string vSrc, std::string fSrc) {vertexSource = vSrc; fragmentSource = fSrc; }
        ShaderSources(const ShaderSources &orig) {vertexSource = orig.vertexSource; fragmentSource = orig.fragmentSource; }
    };

    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string& fp);

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

    int GetUniformLocation(const std::string& name);

private:
    unsigned int CreateShader(std::string& vertexSource, std::string& fragmentSource);
    int CompileShader(unsigned int type, const std::string& source);
    ShaderSources ParseShader(const std::string& fp);
};