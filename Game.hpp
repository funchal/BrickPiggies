#ifndef Game_hpp
#define Game_hpp

#include "Window.hpp"
#include "Program.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "Texture.hpp"

class Game
{
public:
    Game();

    void loop();
    void render();
    void window_resized(int width, int height);

    Window window;
    Program program;
    Camera camera;

    World* world;
    Texture* hazard;

private:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
};

extern Game* game;
extern SDL_Renderer* renderer;

#endif
