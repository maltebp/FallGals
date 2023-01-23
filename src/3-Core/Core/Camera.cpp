#include "Camera.hpp"

#include <sre/Camera.hpp>

#include <Core/Transform.hpp>

using namespace std;
using namespace glm;
using namespace fg::core;


Camera::Camera() {
    transform = make_shared<Transform>();
    sreCamera = make_shared<sre::Camera>();
}


const shared_ptr<Transform>& Camera::getTransform() const {
    return transform;
}


vec3 Camera::getLocalForwardDirection() const {
    return mat3_cast(transform->getLocalRotation()) * vec3(DEFAULT_FORWARD_DIRECTION);
}


vec3 Camera::getLocalRightDirection() const {
    return mat3_cast(transform->getLocalRotation()) * vec3(DEFAULT_RIGHT_DIRECTION);
}


vec3 Camera::getLocalUpDirection() const {
        return mat3_cast(transform->getLocalRotation()) * vec3(DEFAULT_UP_DIRECTION);
}


vec3 Camera::getWorldViewDirection() const {
    mat4 tranposeInverseWorldMatrix = transpose(inverse(transform->getWorldMatrix()));
    return tranposeInverseWorldMatrix * vec4(DEFAULT_FORWARD_DIRECTION, 1.0f);
}


const shared_ptr<sre::Camera>& Camera::updateAndGetSreCamera() {

    vec3 worldPosition = transform->getWorldPosition();

    mat4 tranposeInverseWorldMatrix = transpose(inverse(transform->getWorldMatrix()));

    vec3 worldViewDirection = tranposeInverseWorldMatrix * vec4(DEFAULT_FORWARD_DIRECTION, 1.0f);

    vec3 worldViewTarget = worldPosition + worldViewDirection;

    vec3 worldUpDirection = tranposeInverseWorldMatrix * vec4(DEFAULT_UP_DIRECTION, 1.0f);

    sreCamera->lookAt(worldPosition, worldViewTarget, worldUpDirection);

    sreCamera->setPerspectiveProjection(fieldOfView, nearPlane, farPlane);

    return sreCamera;
}