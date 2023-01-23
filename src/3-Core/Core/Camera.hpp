#pragma once

#include <glm/glm.hpp>

#include <Core/Entity.hpp>

namespace sre {
    class Camera;
}

namespace fg::core {

    class Transform;


    class Camera : public Entity {

        friend Renderer;

    public:

        const std::shared_ptr<Transform>& getTransform() const;

        glm::vec3 getLocalForwardDirection() const;

        glm::vec3 getLocalRightDirection() const;

        glm::vec3 getLocalUpDirection() const;

        glm::vec3 getWorldViewDirection() const;

        const std::shared_ptr<sre::Camera>& updateAndGetSreCamera();

        const glm::vec3 DEFAULT_UP_DIRECTION = glm::vec3{0, 1, 0};
        
        const glm::vec3 DEFAULT_FORWARD_DIRECTION = glm::vec3{0, 0, -1};

        const glm::vec3 DEFAULT_RIGHT_DIRECTION = glm::vec3{1, 0, 0};

        Camera();

    protected:


        std::shared_ptr<Transform> transform;

    private:

        // This is mainly a smart pointer to avoid including sre/Camera.hpp
        std::shared_ptr<sre::Camera> sreCamera;

        float fieldOfView = 60;

        float nearPlane = 0.01f;

        float farPlane = 1000.0f;

    };

}