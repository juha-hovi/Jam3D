#include <iostream>
#include <sstream>
#include <fstream>

#include <GL/glew.h>

#include "shader.h"

Shader::Shader(const std::string& fp)
    : m_FilePath(fp), m_RendererID(0)
{
    ShaderSources shaderSources = ParseShader(m_FilePath);
    m_RendererID = CreateShader(shaderSources.vertexSource, shaderSources.fragmentSource);
}

unsigned int Shader::CreateShader(std::string& vertexSource, std::string& fragmentSource)
{
    unsigned int program = glCreateProgram();
    unsigned int vertexID = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentID = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(program, vertexID);
    glAttachShader(program, fragmentID);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    return program;
}

int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << "\n";
		std::cout << message << "\n";
		glDeleteShader(id);
		return 0;
    }
    std::cout << "Successfully compiled " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    return id;
}

Shader::ShaderSources Shader::ParseShader(const std::string& fp)
{
    std::ifstream stream(fp);
    std::stringstream sstreamVertex;
    std::stringstream sstreamFragment;
    int typeSwitch = -1;

    if (stream.is_open())
    {
        std::string line;
        while (getline(stream, line))
        {
            if (line.find("#Vertex") != line.npos)
            {
                typeSwitch = 1;
                continue;
            }
            else if (line.find("#Fragment") != line.npos)
            {
                typeSwitch = 2;
                continue;
            }

            switch (typeSwitch)
            {
            case 1:
                sstreamVertex << line << "\n";
                break;
            
            case 2:
                sstreamFragment << line << "\n";
                break;

            case -1:
                std::cout << "Shader type not identified. Current line: " << line << std::endl;
            }  
        }
    }

    else 
        std::cout << "Failed to open shader file" << std::endl;

    ShaderSources shaderSources = {sstreamVertex.str(), sstreamFragment.str()};
    return shaderSources;
}