#ifndef Texture_hpp
#define Texture_hpp

#include <GL/glew.h>

class Texture
{
public:
    Texture(const char* filename);
    ~Texture();

    void bind() const;
    void unbind() const;

//private:
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    GLuint texture;
};

#endif
