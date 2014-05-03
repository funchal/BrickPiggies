#include "Shader.hpp"
#include "Error.hpp"
#include <fstream>

Shader::Shader(const char* filename, GLenum shader_type) :
    shader(0)
{
    shader = glCreateShader(shader_type);

    info("Compiling shader %d from '%s'.", shader, filename);

    // read file contents
    std::ifstream in;
    in.exceptions(std::ios_base::badbit | std::ios_base::failbit | std::ios_base::eofbit);
    in.open(filename, std::ios_base::binary | std::ios_base::ate);
    unsigned size = in.tellg();
    in.seekg(0, std::ios_base::beg);
    char* contents = new char[size + 1];
    in.read(contents, size);
    contents[size] = '\0';

    glShaderSource(shader, 1, (const GLchar**) &contents, NULL);
    glCompileShader(shader);
    delete[] contents;

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char logInfo[length];
        glGetShaderInfoLog(shader, length, NULL, logInfo);
        fatal("Error compiling shader:\n%s", logInfo);
    }
}

Shader::~Shader()
{
    glDeleteShader(shader);
}

VertexShader::VertexShader(const char* filename)
: Shader(filename, GL_VERTEX_SHADER)
{}

FragmentShader::FragmentShader(const char* filename)
: Shader(filename, GL_FRAGMENT_SHADER)
{}
