#include "Window.hpp"
#include "Error.hpp"
#include <GL/glew.h>

Window::Window(const char* title, int width, int height, bool fullscr)
: window(NULL), fullscreen(fullscr)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        fatal("Could not initialize SDL: %s", SDL_GetError());
    }

    SDL_version version;
    SDL_GetVersion(&version);
    info("Using SDL version %d.%d.%d.",
         version.major, version.minor, version.patch);

    unsigned flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
    if (fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height, flags);
    if (!window) {
        fatal("Could not create window: %s.", SDL_GetError());
    }

    SDL_DisplayMode display_mode;
    if (SDL_GetWindowDisplayMode(window, &display_mode) != 0) {
        fatal("Could not get display mode: %s.", SDL_GetError());
    }

    info("Using display driver:%s mode:%dx%d %dHz %dbpp.",
         SDL_GetCurrentVideoDriver(),
         display_mode.w, display_mode.h,
         display_mode.refresh_rate,
         SDL_BITSPERPIXEL(display_mode.format));

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    context = SDL_GL_CreateContext(window);
    if (!context) {
        fatal("Could not initialize OpenGL: %s.", SDL_GetError());
    }

    info("Using OpenGL version %s.", glGetString(GL_VERSION));

    if (SDL_GL_SetSwapInterval(-1) != 0) {
        if (SDL_GL_SetSwapInterval(1) != 0) {
            fatal("Cannot set swap interval: %s.", SDL_GetError());
        }
    }

    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        fatal("Could not initialize GLEW: %s.", glewGetErrorString(error));
    }
    glGetError(); // glewInit can cause spurious gl errors, clear error flag

    info("Using GLEW version %s.", glewGetString(GLEW_VERSION));

    SDL_DisableScreenSaver();
    SDL_ShowWindow(window);
}

Window::~Window()
{
    SDL_HideWindow(window);
    SDL_EnableScreenSaver();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::check_gl() const
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        fatal("OpenGL error: %s.", gluErrorString(error));
    }
}

bool Window::is_fullscreen() const
{
    return fullscreen;
}

void Window::set_fullscreen(bool fullscr)
{
    fullscreen = fullscr;
    SDL_SetWindowFullscreen(window,
                            fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
}

void Window::swap_buffers()
{
    SDL_GL_SwapWindow(window);
}
