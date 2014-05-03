#include "Program.hpp"
#include "Shader.hpp"
#include "Error.hpp"

Program::Program()
: program(0)
{
    program = glCreateProgram();
}

Program::~Program()
{
    glDeleteProgram(program);
}

void Program::attach(const Shader& shader)
{
    glAttachShader(program, shader.shader);
}

void Program::link()
{
    info("Linking GLSL program %d.", program);

    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char logInfo[length];
        glGetProgramInfoLog(program, length, NULL, logInfo);
        fatal("Error linking program:\n%s", logInfo);
    }

    GLint num_shaders;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &num_shaders);

    GLuint shaders[num_shaders];
    glGetAttachedShaders(program, num_shaders, NULL, shaders);

    for (GLint i = 0; i != num_shaders; i++) {
        glDetachShader(program, shaders[i]);
    }
}

void Program::bind() const
{
    glUseProgram(program);
}

void Program::unbind() const
{
    glUseProgram(0);
}

Attribute Program::getAttribute(const char* name)
{
    GLint location = glGetAttribLocation(program, name);
    if (location == -1) {
        fatal("Vertex attribute '%s' not found.");
    }
    return location;
}

Uniform Program::getUniform(const char* name)
{
    GLint location = glGetUniformLocation(program, name);
    if (location == -1) {
        fatal("Uniform variable '%s' not found.");
    }
    return location;
}

Attribute::Attribute(GLuint loc)
: location(loc)
{}


void Attribute::enable()
{
    glEnableVertexAttribArray(location);
}

void Attribute::disable()
{
    glDisableVertexAttribArray(location);
}

void Attribute::set(GLint size, GLenum type, GLboolean normalized,
                    GLsizei stride, GLsizei offset)
{
    glVertexAttribPointer(location, size, type, normalized, stride,
                          reinterpret_cast<const GLvoid*>(offset));
}

Uniform::Uniform(GLuint loc)
: location(loc)
{}

void Uniform::set1f(GLfloat value)
{
    glUniform1f(location, value);
}

void Uniform::set1i(GLint value)
{
    glUniform1i(location, value);
}

void Uniform::set_matrix4f(const GLfloat* value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}
