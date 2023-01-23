#pragma once

#include <memory>

#include <Core/Camera.hpp>

namespace fg::core {
    class WindowContext;
}

namespace fg::game {


    class DebugCamera : public core::Camera {
    public:

        DebugCamera(std::weak_ptr<core::WindowContext> windowContext);

        // Degrees per pixel
        void setMoveSpeed(float speed);

        // Degrees per pixel
        void setRotationSpeed(float speed);

    protected:

        void onUpdate(const core::GameTime& gameTime) override;

        bool onKeyEvent(SDL_KeyboardEvent& keyEvent) override;

        bool onMouseButtonEvent(SDL_MouseButtonEvent& mouseButtonEvent) override;

        bool onMouseMotionEvent(SDL_MouseMotionEvent& mouseMotionEvent) override;

    private:

        std::weak_ptr<core::WindowContext> windowContext;

        bool movementEnabled = false;

        float moveSpeed = 4.0f;

        float rotationSpeed = 0.3f;

        float xRotation = 0.0f;

        float yRotation = 0.0f;

        bool moveForward = false;
    
        bool moveBackward = false;
    
        bool moveLeft = false;
    
        bool moveRight = false;

        bool moveUp = false;

        bool moveDown = false;

    };

}