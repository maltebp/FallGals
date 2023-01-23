#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;
using namespace fg::core;

const glm::vec3 DEFAULT_UP_DIRECTION = glm::vec3{ 0, 1, 0 };

const glm::vec3 DEFAULT_FORWARD_DIRECTION = glm::vec3{ 0, 0, -1 };

const glm::vec3 DEFAULT_RIGHT_DIRECTION = glm::vec3{ 1, 0, 0 };

Transform::Transform() {
    
}


Transform::Transform(glm::vec3 position, glm::vec3 scale, glm::quat rotation)
    :   localPosition(position),
        localScale(scale),
        localRotation(rotation)
{ }
        

void Transform::setParent(weak_ptr<Transform> parent) {
    this->parent = parent;
}


void Transform::clearParent() {
    parent.reset();
}


const weak_ptr<Transform>& Transform::getParent() const {
    return parent;
}


mat4 Transform::getModelMatrix() const {
    mat4 matrix = mat4(1.0f);

    matrix = translate(matrix, localPosition);   
    matrix = matrix * mat4_cast(localRotation);
    matrix = scale(matrix, localScale);

    return matrix;
}


mat4 Transform::getWorldMatrix() const {

    mat4 modelMatrix = getModelMatrix();

    if( shared_ptr<Transform> parentSharedPtr = parent.lock() ) {
        return parentSharedPtr->getWorldMatrix() * modelMatrix;
    }

    return modelMatrix;
}


void Transform::setLocalPosition(vec3 position) {
    localPosition = position;
}

void fg::core::Transform::setLocalPositionByWorldPosition(glm::vec3 worldPosition)
{
    if (shared_ptr<Transform> parentPtr = parent.lock()) {
        auto pPos = parentPtr->getWorldPosition();
        worldPosition = worldPosition - pPos;
    }
    localPosition = worldPosition;
}


void Transform::adjustLocalPosition(vec3 adjustment) {
    localPosition += adjustment;
}


vec3 Transform::getLocalPosition() const {
    return localPosition;
}


void Transform::setLocalScale(vec3 scale) {
    assert(
        scale.x > 0 && scale.y > 0 && scale.z > 0 &&
        "Transform: scale must be larger than 0"
    );

    localScale = scale;
}


vec3 Transform::getLocalScale() const {
    return localScale;
}


void Transform::setLocalRotation(quat rotation) {
    localRotation = rotation;
}


quat Transform::getLocalRotation() const {
    return localRotation;
}


vec3 Transform::getLocalXAxis() const {
    return mat3_cast(localRotation) * vec3(1, 0, 0);
}


vec3 Transform::getLocalYAxis() const {
    return mat3_cast(localRotation) * vec3(0, 1, 0);
}


vec3 Transform::getLocalZAxis() const {
    return mat3_cast(localRotation) * vec3(0, 0, 1);
}


vec3 Transform::getWorldPosition() const {
    if( shared_ptr<Transform> parentPtr = parent.lock() ) {
        return parentPtr->getWorldMatrix() * vec4(localPosition, 1.0f);
    }

    return localPosition;
}

glm::vec3 fg::core::Transform::getLocalForwardDirection() const
{
    return mat3_cast(getLocalRotation()) * vec3(DEFAULT_FORWARD_DIRECTION);
}
