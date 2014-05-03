#include "Texture.hpp"
#include "Error.hpp"
#include "lodepng.h"
#include <cstdlib>

Texture::Texture(const char* filename)
: texture(0)
{
    glGenTextures(1, &texture);

    info("Loading texture %d from file '%s'.", texture, filename);

    unsigned width, height;
    unsigned char* image;
    unsigned error = lodepng_decode32_file(&image, &width, &height, filename);
    if (error) {
        fatal("Could not decode file '%s': %s.",
              filename, lodepng_error_text(error));
    }

    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 /*lod*/ 0,
                 GL_RGBA,
                 width, height,
                 /*border*/ 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 image);

    unbind();
    free(image);
}

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
