#ifndef Game_hpp
#define Game_hpp

#include "Window.hpp"
#include "Program.hpp"
#include "Camera.hpp"

class Game
{
public:
    Game();

    void loop();
    void render();
    void window_resized(int width, int height);

private:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    Window window;
    Program program;
    Camera camera;
};

#endif
