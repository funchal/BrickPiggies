#include "World.hpp"
#include "Game.hpp"
#include "Program.hpp"

World::World()
{
    // TODO: generate geometry in its own function
    // TODO: cull faces touching each other
    // TODO: cull back faces
    // TODO: draw front to back?

    unsigned y = 10;
    for (unsigned x = 0; x != 128; x++) {
        for (unsigned z = 0; z != 128; z++) {
            for (unsigned k = y; k != -1; k--) {
                blocks[x][k][z] = 1;
            }
        }
    }

    // the Vertex Array links vertex data to shader variables
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    game->window.check_gl();

    // the Vertex Buffer stores a copy of vertex data in video memory
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    game->window.check_gl();

    GLfloat vertex_data[] =
    {
     //  X     Y     Z       U     V
     // bottom
     -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
     -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
     1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
     -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,

     // top
     -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
     -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
     1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
     -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

     // front
     -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
     1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
     -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
     1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
     -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

     // back
     -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
     -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
     -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
     1.0f, 1.0f,-1.0f,   1.0f, 1.0f,

     // left
     -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
     -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
     -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
     -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
     -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
     -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,

     // right
     1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
     1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
     1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
     1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
     1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
     1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    game->window.check_gl();
}

void World::render()
{
    game->program.bind();
    glBindVertexArray(vao);
    game->window.check_gl();

    glActiveTexture(GL_TEXTURE0);
    game->hazard->bind();

    Attribute vert = game->program.getAttribute("vert");
    vert.enable();
    vert.set(3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);
    game->window.check_gl();

    Attribute texcoord = game->program.getAttribute("vertTexCoord");
    texcoord.enable();
    texcoord.set(2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 3*sizeof(GLfloat));
    game->window.check_gl();

    Uniform tex = game->program.getUniform("tex");
    tex.set1i(0); //set to 0 because the texture is bound to GL_TEXTURE0 unit

    Uniform proj = game->program.getUniform("projection");
    glm::mat4 projection = game->camera.get_projection();
    proj.set_matrix4f(&projection[0][0]);

    for (unsigned x = 0; x != 128; x++) {
        for (unsigned y = 0; y != 128; y++) {
            for (unsigned z = 0; z != 128; z++) {
                if (blocks[x][y][z]) {
                    Uniform cam = game->program.getUniform("camera");
                    glm::mat4 cam_matrix = game->camera.get_view();
                    cam_matrix = glm::translate(cam_matrix,
                                                glm::vec3(2.f*x, 2.f*y, 2.f*z));
                    cam.set_matrix4f(&cam_matrix[0][0]);

                    glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
                    game->window.check_gl();
                }
            }
        }
    }

    game->hazard->unbind();
    game->program.unbind();
    game->window.check_gl();
}
