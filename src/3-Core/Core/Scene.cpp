#include "Scene.hpp"

using namespace fg::core;


void Scene::update(const GameTime& time) {
    onUpdate(time);
    rootEntity.update(time);
}


void Scene::render(Renderer& renderer) {
    onRender(renderer);
    rootEntity.render(renderer);
}


void Scene::renderGui() {
    onRenderGui();
    rootEntity.renderGui();
}


bool Scene::keyEvent(SDL_KeyboardEvent& keyboardEvent) {
    bool handled = onKeyEvent(keyboardEvent);
    if( handled ) return true;
    return rootEntity.keyEvent(keyboardEvent);
}


bool Scene::mouseMotionEvent(SDL_MouseMotionEvent& mouseMotionEvent) {
    bool handled = onMouseMotionEvent(mouseMotionEvent);
    if( handled ) return true;
    return rootEntity.mouseMotionEvent(mouseMotionEvent);
}


bool Scene::mouseButtonEvent(SDL_MouseButtonEvent& mouseButtonEvent) {
    bool handled = onMouseButtonEvent(mouseButtonEvent);
    if( handled ) return true;
    return rootEntity.mouseButtonEvent(mouseButtonEvent);
}