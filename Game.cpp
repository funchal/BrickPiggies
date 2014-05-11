#include "Game.hpp"
#include "Error.hpp"
#include "Texture.hpp"
#include <SDL2/SDL_ttf.h>
#include <string>

Game* game = NULL;

SDL_Renderer* renderer;

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

    world = new World();
}

void Game::loop()
{
    glm::vec3 camera_delta;
    float orbitate_delta = 0;
    float zoom_delta = 0;

    Uint32 curr_time = SDL_GetTicks();
    while (true) {
        // TODO: fps count
        // TODO: better keyboard handling
        // TODO: asset (texture, shader) management
        // TODO: pick blocks with raycast

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

        // TODO: move this logic into the camera class, make it smooth
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

SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
    SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
    //Open the font
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr){
        fatal("TTF_OpenFont:%s, %s.", fontFile.c_str(), TTF_GetError());
        return nullptr;
    }
    //We need to first render to a surface as that's what TTF_RenderText
    //returns, then load that surface into a texture
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    if (surf == nullptr){
        TTF_CloseFont(font);
        //logSDLError(std::cout, "TTF_RenderText");
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr){
        //logSDLError(std::cout, "CreateTexture");
    }
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

void Game::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    window.check_gl();

    world->render();
    window.check_gl();

    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Texture* text = renderText("blabla", "/home/helltone/projects/BrickPiggies/Resources/sample.ttf", color, 64, renderer);
    SDL_Rect dst;
    dst.x = 10;
    dst.y = 10;
    SDL_QueryTexture(text, NULL, NULL, &dst.w, &dst.h);
    window.check_gl();
    SDL_RenderCopy(renderer, text, NULL, &dst);
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
        game = new Game();
        game->loop();
        return EXIT_SUCCESS;
    } catch(...) {
        return EXIT_FAILURE;
    }
}
