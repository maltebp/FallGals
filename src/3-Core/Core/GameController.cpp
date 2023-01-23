#include "GameController.hpp"

#include <cassert>

#include <SDL_events.h>
#include <sre/SDLRenderer.hpp>
#include <sre/Inspector.hpp>

#include <Core/WindowContext.hpp>
#include <Core/Scene.hpp>
#include <Core/GameTime.hpp>
#include <Core/Rigidbody.hpp>

using namespace std;
using namespace sre;
using namespace fg::core;


void GameController::run(shared_ptr<Scene> startScene) {
    assert(startScene != nullptr && "Scene must not be null");
    currentScene = startScene;

    SDLRenderer sreRenderer;
    sreRenderer.setWindowSize({800, 600});
    
    sreRenderer.init()
      .withSdlInitFlags(SDL_INIT_EVERYTHING)
      .withSdlWindowFlags(SDL_WINDOW_OPENGL);

    Physics::initWorld(glm::vec3(0, -9.8, 0));

    windowContext = make_shared<WindowContext>(sreRenderer.getSDLWindow());

    sreRenderer.frameUpdate = [&](float deltaTime){
        sreRendererOnUpdate(deltaTime);
    };

    sreRenderer.frameRender = [&]() {
        sreRendererOnRender();
    };

    sreRenderer.keyEvent = [&](SDL_Event& sdlEvent) {
        sreRendererOnKeyEvent(sdlEvent);
    };

    sreRenderer.mouseEvent = [&](SDL_Event& sdlEvent) {
        sreRendererOnMouseEvent(sdlEvent);
    };

    currentScene->onStart();

    // start game loop
    sreRenderer.startEventLoop();
}


void GameController::setScene(shared_ptr<Scene> newScene) {
    assert(newScene != nullptr && "Scene must not be null");
    queuedScene = newScene;
}


const std::shared_ptr<WindowContext>& GameController::getWindowContext() const {
    return windowContext;
}


void GameController::sreRendererOnUpdate(float deltaTime) {

    if( sreInspectorIsEnabled ) {
        sreInspector->update();
    }

    GameTime gameTime;
    gameTime.frameTime = deltaTime;

    if( queuedScene != nullptr ) {
        currentScene->onEnd();
        currentScene = queuedScene;
        queuedScene = nullptr;
        currentScene->onStart();
    }

    Physics::getInstance()->update(deltaTime);

    currentScene->update(gameTime);
    currentScene->rootEntity.clean();
}


void GameController::sreRendererOnRender() {
    renderer.renderScene(*currentScene, sreInspector.get());
}


void GameController::sreRendererOnKeyEvent(SDL_Event& sdlEvent) {
    assert(sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP);

    SDL_KeyboardEvent keyEvent = sdlEvent.key;

    if( keyEvent.keysym.sym == SDLK_i && keyEvent.type == SDL_KEYDOWN) {
        sreInspectorIsEnabled = !sreInspectorIsEnabled;
        if( sreInspectorIsEnabled && sreInspector == nullptr) {
            sreInspector = make_shared<sre::Inspector>();
        }
        else if (!sreInspectorIsEnabled) {
            sreInspector = nullptr;
        }
    }
    else {
        currentScene->keyEvent(keyEvent);
    }
}


void GameController::sreRendererOnMouseEvent(SDL_Event& sdlEvent) {
    
    switch(sdlEvent.type) {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            SDL_MouseButtonEvent mouseButtonEvent = sdlEvent.button;
            currentScene->mouseButtonEvent(mouseButtonEvent);
            return;

        case SDL_MOUSEMOTION:
            SDL_MouseMotionEvent mouseMotionEvent = sdlEvent.motion;
            currentScene->mouseMotionEvent(mouseMotionEvent);
            return;

        case SDL_MOUSEWHEEL:
            // We do not support this yet
            return;
    }

    assert(false && "Unexpected SDL mouse event");  
}
