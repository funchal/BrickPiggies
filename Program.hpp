#ifndef Program_hpp
#define Program_hpp

#include "Shader.hpp"
#include <GL/glew.h>

class Attribute
{
public:
    void enable();
    void disable();

    void set(GLint size, GLenum type, GLboolean normalized,
             GLsizei stride, GLsizei offset);

private:
    friend class Program;

    Attribute() = delete;

    Attribute(GLuint location);

    GLuint location;
};

class Uniform
{
public:
    void set1f(GLfloat value);
    void set1i(GLint value);
    void set_matrix4f(const GLfloat* value);

private:
    friend class Program;

    Uniform() = delete;

    Uniform(GLuint location);

    GLuint location;
};

class Program
{
public:
    Program();
    ~Program();

    void attach(const Shader& shader);
    void link();
    void bind() const;
    void unbind() const;

    Attribute getAttribute(const char* name);
    Uniform getUniform(const char* name);

private:
    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    GLuint program;
};

#endif
