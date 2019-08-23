#include "shader.h"
#include "log.h"

#include <sstream>
#include <fstream>

#include <GL/glew.h>

namespace Jam3D {

// Creates OpenGL shader program from text file.
// Input:
//    - fp: reference to a filepath string
Shader::Shader(const std::string& fp, unsigned int type)
    : m_FilePath(fp), m_RendererID(0), m_Type(type)
{
    ShaderSources shaderSources = ParseShader(m_FilePath);
    m_RendererID = CreateShader(shaderSources);
}

// Compiles the shaders and links them to a program.
// Input:
//    - vertexSource: reference to a vertex shader source string
//    - fragmentSource: reference to a fragment shader source string
// Output:
//    - program ID associated with the shaders
unsigned int Shader::CreateShader(ShaderSources src)
{
    if (m_Type == VERTEX_FRAGMENT)
    {
        unsigned int program = glCreateProgram();
        unsigned int vertexID = CompileShader(GL_VERTEX_SHADER, src.vertexSource);
        unsigned int fragmentID = CompileShader(GL_FRAGMENT_SHADER, src.fragmentSource);

        glAttachShader(program, vertexID);
        glAttachShader(program, fragmentID);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);

        return program;
    }
    
    else if (m_Type == VERTEX_GEOMETRY_FRAGMENT)
    {
        unsigned int program = glCreateProgram();
        unsigned int vertexID = CompileShader(GL_VERTEX_SHADER, src.vertexSource);
        unsigned int fragmentID = CompileShader(GL_FRAGMENT_SHADER, src.fragmentSource);
        unsigned int geometryID = CompileShader(GL_GEOMETRY_SHADER, src.geometrySource);
        glAttachShader(program, vertexID);
        glAttachShader(program, geometryID);
        glAttachShader(program, fragmentID);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vertexID);
        glDeleteShader(geometryID);
        glDeleteShader(fragmentID);

        return program;
    }
    return 0;
}

// Compiles a single shader and reports failure.
// Input:
//    - type: GL_ENUM representing shader type
//    - source: reference to a shader source string
// Output:
//    - id: shader id of a successfully compiled shader
//    - 0 if compilation failed
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

        if (type == GL_VERTEX_SHADER)
		    Jam3D::Log::Warning("Failed to compile vertex shader!");
        if (type == GL_GEOMETRY_SHADER)
		    Jam3D::Log::Warning("Failed to compile geometry shader!");
        if (type == GL_FRAGMENT_SHADER)
		    Jam3D::Log::Warning("Failed to compile fragment shader!");

		Jam3D::Log::Warning(message);

		glDeleteShader(id);
		return 0;
    }
    return id;
}

// Parses a single text file containing vertex and fragment shader source codes.
// See 'shader.basic' file for example on format.
// Input:
//    - fp: reference to file path string
// Output:
//    - id: shader id of a successfully compiled shader
//    - 0 if compilation failed
Shader::ShaderSources Shader::ParseShader(const std::string& fp)
{
    std::ifstream stream(fp);
    std::stringstream sstreamVertex;
    std::stringstream sstreamGeometry;
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
            else if (line.find("#Geometry") != line.npos)
            {
                typeSwitch = 3;
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
            case 3:
                sstreamGeometry << line << "\n";
                break;
            case -1:
                Jam3D::Log::Warning((std::string("Shader type not identified. Current line: ") + line).c_str());
            }  
        }
    }

    else 
        Jam3D::Log::Warning(std::string("Failed to open shader file in: " + fp));

    ShaderSources shaderSources = {sstreamVertex.str(), sstreamGeometry.str(), sstreamFragment.str()};
    return shaderSources;
}

// Binds the program.
void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

// Unbinds the program.
void Shader::Unbind() const
{
    glUseProgram(0);
}

// Sets value for a uniform containing 1 int.
void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

// Sets value for a uniform containing 1 float.
void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

// Sets values for a uniform containing 3 floats.
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

// Sets values for a uniform containing 4 floats.
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

// Sets values for a uniform containing a 4x4 matrix.
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

// Returns the location of uniform if it exists and
// maintains a cache of uniform locations.
int Shader::GetUniformLocation(const std::string& name)
{
    // If uniform location is cached, return it.
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    // If uniform location is not cached, cache and return it if it exists.
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        Jam3D::Log::Warning(std::string("Warning: uniform " + name + " doesn't exist"));

    m_UniformLocationCache[name] = location;
    return location;
}

}