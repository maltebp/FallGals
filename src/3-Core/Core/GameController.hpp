#pragma once

#include <memory>

#include <Core/Renderer.hpp>
#include <Core/Physics.hpp>

union SDL_Event;

namespace sre {
    class Inspector;
}

namespace fg::core {

    class WindowContext;
    class Scene;


    class GameController {
    public:

        ~GameController() = default;

        void run(std::shared_ptr<Scene> startScene);

        void setScene(std::shared_ptr<Scene> newScene);

        const std::shared_ptr<WindowContext>& getWindowContext() const;
        
    private:

        void sreRendererOnUpdate(float deltaTime);

        void sreRendererOnRender();

        void sreRendererOnKeyEvent(SDL_Event& keyEvent);

        void sreRendererOnMouseEvent(SDL_Event& mouseEvent);

        std::shared_ptr<Scene> currentScene;

        std::shared_ptr<Scene> queuedScene;

        std::shared_ptr<WindowContext> windowContext;

        Renderer renderer;

        bool sreInspectorIsEnabled = false;

        std::shared_ptr<sre::Inspector> sreInspector;

    };

}