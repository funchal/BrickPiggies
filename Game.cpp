#include "Game.hpp"
#include "Error.hpp"
#include "Texture.hpp"

Texture* hazard;

Game::Game()
: window("Game", 1024, 800, false)
{
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

    glm::vec3 camera_delta;
    float orbitate_delta = 0;
    float zoom_delta = 0;

    Uint32 curr_time = SDL_GetTicks();
    while (true) {
        Uint32 next_time = SDL_GetTicks();
        Uint32 delta_time = next_time - curr_time + 1;
        curr_time = next_time;

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
                    switch (event.key.keysym.sym) {
                        case SDLK_RETURN:
                            if (event.key.keysym.mod & KMOD_ALT) {
                                window.set_fullscreen(!window.is_fullscreen());
                            }
                            break;
                        case SDLK_ESCAPE:
                            return;
                        case SDLK_q:
                            orbitate_delta += 65;
                            break;
                        case SDLK_e:
                            orbitate_delta -= 65;
                            break;
                        case SDLK_a:
                        case SDLK_LEFT:
                            camera_delta += 10.f*camera.get_left();
                            break;
                        case SDLK_d:
                        case SDLK_RIGHT:
                            camera_delta -= 10.f*camera.get_left();
                            break;
                        case SDLK_w:
                        case SDLK_UP:
                            camera_delta += 10.f*camera.get_forward();
                            break;
                        case SDLK_s:
                        case SDLK_DOWN:
                            camera_delta -= 10.f*camera.get_forward();
                            break;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    zoom_delta += 10*event.wheel.y;
                    break;
            }
        }

        camera.move(camera_delta*(0.01f*delta_time));
        camera.orbitate(orbitate_delta*(0.01f*delta_time));
        camera.zoom(zoom_delta*(0.01f*delta_time));

        camera_delta.x = 10*camera_delta.x/delta_time;
        camera_delta.y = 10*camera_delta.y/delta_time;
        camera_delta.z = 10*camera_delta.z/delta_time;
        orbitate_delta = 10*orbitate_delta/delta_time;
        zoom_delta = 10*zoom_delta/delta_time;

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

    Uniform cam = program.getUniform("camera");
    glm::mat4 cam_matrix = camera.get_view();
    cam.set_matrix4f(&cam_matrix[0][0]);

    Uniform proj = program.getUniform("projection");
    glm::mat4 projection = camera.get_projection();
    proj.set_matrix4f(&projection[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
    window.check_gl();

    hazard->unbind();
    program.unbind();
    window.check_gl();

    window.swap_buffers();
    window.check_gl();
}

void Game::window_resized(int width, int height)
{
    info("Resizing window %dx%d.", width, height);
    camera.set_aspect_ratio((float)width / height);
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
