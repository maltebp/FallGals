#pragma once

#include <Core/EntityGroup.hpp>

struct SDL_KeyboardEvent;
struct SDL_MouseButtonEvent;
struct SDL_MouseMotionEvent;

namespace fg::core {

    class GameTime;
    class GameController;
    class Renderer;
    class Camera;


    class Scene {

        friend GameController;
        friend Renderer;

    public:

        std::weak_ptr<Camera> activeCamera;

    protected:

        Scene(GameController& gameController) 
            :   gameController(gameController)
        { }

        virtual void onStart() { }

        virtual void onUpdate(const GameTime& time) { }

        virtual void onRender(Renderer& renderer) { }

        virtual void onRenderGui() { }
        
        // Returns true if event is handled
        virtual bool onKeyEvent(SDL_KeyboardEvent& keyboardEvent) { return false; }

        // Returns true if event is handled
        virtual bool onMouseMotionEvent(SDL_MouseMotionEvent& mouseMotionEvent) { return false; }

        // Returns true if event is handled
        virtual bool onMouseButtonEvent(SDL_MouseButtonEvent& mouseButtonEvent) { return false; }

        virtual void onEnd() { }

        EntityGroup rootEntity;

        GameController& gameController;

    private:

        void update(const GameTime& time);

        void render(Renderer& renderer);

        void renderGui();

        // Returns true if event is handled
        bool keyEvent(SDL_KeyboardEvent& keyboardEvent);

        // Returns true if event is handled
        bool mouseMotionEvent(SDL_MouseMotionEvent& mouseMotionEvent);

        // Returns true if event is handled
        bool mouseButtonEvent(SDL_MouseButtonEvent& mouseButtonEvent);

    };

}