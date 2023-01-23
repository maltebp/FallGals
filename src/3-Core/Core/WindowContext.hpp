#pragma once

struct SDL_Window;

namespace fg::core {


    // Wraps window/input access into the simplified class
    class WindowContext {
    public:

        WindowContext(SDL_Window* sdlWindow);

        // Locks the cursor to center of screen, and hides it
        void enableMouseLock();

        void disableMouseLock();

    private:

        bool relativeMouseMode = false;

        SDL_Window* sdlWindow;
        
    };

}