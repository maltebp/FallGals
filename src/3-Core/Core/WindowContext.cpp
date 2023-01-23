#include "WindowContext.hpp"

#include <cassert>

#include <sre/SDLRenderer.hpp>

using namespace std;
using namespace fg::core;


WindowContext::WindowContext(SDL_Window* sdlWindow)
    :   sdlWindow(sdlWindow)
{
    assert(sdlWindow != nullptr && "SDL Window must not be null");
}


void WindowContext::enableMouseLock() {
    SDL_SetWindowGrab(sdlWindow, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}


void WindowContext::disableMouseLock() {
    SDL_SetWindowGrab(sdlWindow, SDL_FALSE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
}