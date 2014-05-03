#include "Error.hpp"
#include <SDL2/SDL.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <exception>

void fatal(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    int category = SDL_LOG_CATEGORY_APPLICATION;
    SDL_LogPriority priority = SDL_LOG_PRIORITY_CRITICAL;
    SDL_LogMessageV(category, priority, format, ap);
    va_end(ap);

#ifndef NO_STACKTRACE
    char pid_buf[30];
    sprintf(pid_buf, "%d", getpid());
    char name_buf[512];
    name_buf[readlink("/proc/self/exe", name_buf, 511)] = 0;
    prctl(PR_SET_PTRACER, PR_SET_PTRACER_ANY, 0, 0, 0);
    int child_pid = fork();
    if (!child_pid) {
        dup2(STDERR_FILENO, STDIN_FILENO); // redirect output to stderr
        execlp("gdb", "gdb", "--batch", "-n", "-ex", "thread", "-ex",
               "bt", name_buf, pid_buf, NULL);
        abort(); // if gdb failed to start
    } else {
        waitpid(child_pid, NULL, 0);
    }
#endif

    throw std::exception();
}

void info(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    int category = SDL_LOG_CATEGORY_APPLICATION;
    SDL_LogPriority priority = SDL_LOG_PRIORITY_INFO;
    SDL_LogMessageV(category, priority, format, ap);
    va_end(ap);
}
