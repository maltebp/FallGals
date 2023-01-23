#include "Entity.hpp"

#include <SDL_events.h>

using namespace fg::core;


Entity::Entity() { }


Entity::~Entity() {
    destroyAllChildren();
    clean();
}


const std::vector<std::shared_ptr<Entity>>& Entity::getChildren() const {
    return children;
};


std::vector<std::shared_ptr<Entity>> Entity::getChildrenCopy() {
    return children;
}


void Entity::destroy() {
    isMarkedForDestruction = true;
}


void Entity::update(const GameTime& time) {

    onUpdate(time);

    for( std::shared_ptr<Entity>& child : children ) {
        child->update(time);
    }
}


void Entity::render(Renderer& renderer) {
    onRender(renderer);

    for( std::shared_ptr<Entity>& child : children ) {
        child->render(renderer);
    }
}


void Entity::renderGui() {
    onRenderGui();

    for( std::shared_ptr<Entity>& child : children ) {
        child->renderGui();
    }
}


bool Entity::keyEvent(SDL_KeyboardEvent& keyboardEvent) {
    bool handled = onKeyEvent(keyboardEvent);
    if( handled ) return true;

    for( std::shared_ptr<Entity>& child : children ) {
        handled = child->keyEvent(keyboardEvent);
        if( handled ) return true;
    }

    return false;
}


bool Entity::mouseMotionEvent(SDL_MouseMotionEvent& mouseMotionEvent) {
    bool handled = onMouseMotionEvent(mouseMotionEvent);
    if( handled ) return true;

    for( std::shared_ptr<Entity>& child : children ) {
        handled = child->mouseMotionEvent(mouseMotionEvent);
        if( handled ) return true;
    }

    return false;
}


bool Entity::mouseButtonEvent(SDL_MouseButtonEvent& mouseButtonEvent) {
    bool handled = onMouseButtonEvent(mouseButtonEvent);
    if( handled ) return true;

    for( std::shared_ptr<Entity>& child : children ) {
        handled = child->mouseButtonEvent(mouseButtonEvent);
        if( handled ) return true;
    }

    return false;
}


void Entity::destroyAllChildren() {
    for (std::shared_ptr<Entity>& child : children) {
        child->destroy();
        child->destroyAllChildren();
    }
}


void Entity::clean() {

    for( std::shared_ptr<Entity>& child : children ) {
        child->clean();
    }

    children.erase(
        std::remove_if(
            children.begin(), 
            children.end(), 
            [](const std::shared_ptr<Entity>& child) { 
                return child->isMarkedForDestruction;
            }
        ),
        children.end()
    );
}