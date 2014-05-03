#ifndef Shader_hpp
#define Shader_hpp

#include <GL/glew.h>

class Shader
{
public:
    ~Shader();

protected:
    Shader(const char* filename, GLenum shader_type);

private:
    friend class Program;

    Shader() = delete;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    GLuint shader;
};

class VertexShader
: public Shader
{
public:
    explicit VertexShader(const char* filename);
};

class FragmentShader
: public Shader
{
public:
    explicit FragmentShader(const char* filename);
};

#endif
