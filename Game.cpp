#include "Game.hpp"
#include "Error.hpp"
#include "Texture.hpp"

Texture* hazard;

Game::Game()
: window("Game", 1024, 800, false)
{
    scale = 1.0f;
    window_resized(1024, 800);
    program.attach(VertexShader("Resources/Vertex.glsl"));
    program.attach(FragmentShader("Resources/Fragment.glsl"));
    program.link();
    window.check_gl();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    window.check_gl();

    hazard = new Texture("Resources/hazard.png");
    window.check_gl();
}

GLuint vao;

void Game::loop()
{
    // the Vertex Array links vertex data to shader variables
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    window.check_gl();

    // the Vertex Buffer stores a copy of vertex data in video memory
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    window.check_gl();

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
    window.check_gl();

    Attribute vert = program.getAttribute("vert");
    vert.enable();
    vert.set(3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);
    window.check_gl();

    Attribute texcoord = program.getAttribute("vertTexCoord");
    texcoord.enable();
    texcoord.set(2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 3*sizeof(GLfloat));
    window.check_gl();

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_WINDOWEVENT: {
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            window_resized(event.window.data1,
                                           event.window.data2);
                            break;
                    }
                    break;
                }
                case SDL_QUIT:
                    return;
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_RETURN &&
                            event.key.keysym.mod & KMOD_ALT) {
                        window.set_fullscreen(!window.is_fullscreen());
                    } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        return;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0) {
                        scale /= 1.05 * event.wheel.y;
                    } else {
                        scale *= -1.05 * event.wheel.y;
                    }
                    break;
            }
        }

        render();
    }
}

void Game::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    window.check_gl();

    program.bind();
    glBindVertexArray(vao);
    window.check_gl();

    glActiveTexture(GL_TEXTURE0);
    hazard->bind();

    Uniform tex = program.getUniform("tex");
    tex.set1i(0); //set to 0 because the texture is bound to GL_TEXTURE0 unit

    glm::mat4 camera = glm::lookAt(glm::vec3(3*scale, 3*scale, 3*scale),
                                   glm::vec3(0, 0, 0),
                                   glm::vec3(0, 1, 0));

    Uniform cam = program.getUniform("camera");
    cam.set_matrix4f(&camera[0][0]);

    Uniform proj = program.getUniform("projection");
    proj.set_matrix4f(&projection[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
    window.check_gl();

    hazard->unbind();
    program.unbind();

    window.swap_buffers();
}

void Game::window_resized(int width, int height)
{
    info("Resizing window %dx%d.", width, height);
    float fovy = 45.0f;
    float aspect = (float)width/height;
    float near = 0.1f;
    float far = 1024.0f;
    projection = glm::perspective<float>(fovy, aspect, near, far);
    glViewport(0, 0, width, height);
    window.check_gl();
}

int main(int, char*[])
{
    try {
        Game game;
        game.loop();
        return EXIT_SUCCESS;
    } catch(...) {
        return EXIT_FAILURE;
    }
}
