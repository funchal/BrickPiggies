#ifndef World_hpp
#define World_hpp

#include <GL/glew.h>

class World
{
public:
    World();

    void render();

private:
    World(const World&) = delete;
    World& operator=(const World&) = delete;

    char blocks[128][128][128];
    GLuint vao;
};

#endif
