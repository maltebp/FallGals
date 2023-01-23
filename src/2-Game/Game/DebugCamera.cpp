#include "DebugCamera.hpp"

#include <cassert>
#include <iostream>

#include <SDL_events.h>

#include <Utility/Math.hpp>
#include <Core/GameTime.hpp>
#include <Core/WindowContext.hpp>
#include <Core/Transform.hpp>

using namespace std;
using namespace glm;
using namespace fg::utility;
using namespace fg::core;
using namespace fg::game;


DebugCamera::DebugCamera(weak_ptr<WindowContext> windowContext) 
    :   windowContext(windowContext)
{ }


void DebugCamera::setMoveSpeed(float speed) {
    assert(speed > 0 && "Move Speed must be larger than 0");    
    moveSpeed = speed;
}


void DebugCamera::setRotationSpeed(float speed) {
    assert(speed > 0 && "Rotation speed must be larger than 0");
    rotationSpeed = speed;
}


void DebugCamera::onUpdate(const GameTime& gameTime) {
    if( !movementEnabled ) return;

    vec3 moveDirection { 0 };

    if( moveForward ) {
        moveDirection += getLocalForwardDirection();
    }
    if( moveBackward ) {
        moveDirection -= getLocalForwardDirection();
    }
    if( moveRight ) {
        moveDirection += getLocalRightDirection();
    }
    if( moveLeft ) {
        moveDirection -= getLocalRightDirection();
    }
    if( moveUp ) {
        moveDirection += getLocalUpDirection();
    }
    if( moveDown ) {
        moveDirection -= getLocalUpDirection();
    }
    
    moveDirection = length(moveDirection) != 0 ? normalize(moveDirection) : moveDirection;
    vec3 movement = moveDirection * moveSpeed * gameTime.frameTime;
    getTransform()->adjustLocalPosition(movement);
}


bool DebugCamera::onKeyEvent(SDL_KeyboardEvent& keyEvent) {

    if( !movementEnabled ) return false;

    bool keyIsDown = keyEvent.type == SDL_KEYDOWN;

    switch(keyEvent.keysym.sym ) {
        case SDLK_w:
            moveForward = keyIsDown; 
            return true;
        case SDLK_s:
            moveBackward = keyIsDown; 
            return true;
        case SDLK_a:
            moveLeft = keyIsDown; 
            return true;
        case SDLK_d:
            moveRight = keyIsDown; 
            return true;
        case SDLK_e:
            moveUp = keyIsDown; 
            return true;
        case SDLK_q:
            moveDown = keyIsDown; 
            return true;
    }

    return false;
}


bool DebugCamera::onMouseButtonEvent(SDL_MouseButtonEvent& mouseButtonEvent) {
    if( mouseButtonEvent.button != SDL_BUTTON_RIGHT ) return false;

    if( mouseButtonEvent.type == SDL_MOUSEBUTTONDOWN ) {
        movementEnabled = true;
        if( shared_ptr<WindowContext> windowContextPtr = windowContext.lock() ) {
            windowContextPtr->enableMouseLock();
        }
    }

    if( mouseButtonEvent.type == SDL_MOUSEBUTTONUP ) {
        movementEnabled = false;
        if( shared_ptr<WindowContext> windowContextPtr = windowContext.lock() ) {
            windowContextPtr->disableMouseLock();
        }

        moveForward = false;
        moveBackward = false;
        moveRight = false;
        moveLeft = false;
        moveUp = false;
        moveDown = false;
    }

    return true;
}


bool DebugCamera::onMouseMotionEvent(SDL_MouseMotionEvent& mouseMotionEvent) {
    if( !movementEnabled ) return false; 

    float yMovement = mouseMotionEvent.yrel;
    xRotation += -yMovement * rotationSpeed;
    Math::wrapDegrees(xRotation);
    
    float xMovement = mouseMotionEvent.xrel;
    yRotation += -xMovement * rotationSpeed;
    Math::wrapDegrees(yRotation);

    quat rotationQuat = quat(radians(vec3{xRotation, yRotation, 0.0f}));
    getTransform()->setLocalRotation(rotationQuat);

    return true;
}