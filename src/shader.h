#pragma once

#include <string>

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

public:
    Shader(const std::string& fp);

    void Bind() const;
    void Unbind() const;

private:
    unsigned int CreateShader(std::string& vertexSource, std::string& fragmentSource);
    int CompileShader(unsigned int type, const std::string& source);
    ShaderSources ParseShader(const std::string& fp);
};