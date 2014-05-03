#ifndef Window_hpp
#define Window_hpp

#include <SDL2/SDL.h>

class Window
{
public:
    Window(const char* title, int width, int height, bool fullscr);
    ~Window();

    void check_gl() const;

    bool is_fullscreen() const;
    void set_fullscreen(bool fullscr);

    void swap_buffers();

private:
    friend class Context;
    Window();
    Window(const Window&);
    Window& operator=(const Window&);

    SDL_Window* window;
    SDL_GLContext context;

    bool fullscreen;
};

#endif
