#pragma once

#include <memory>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


namespace fg::core {

    class Transform {
    public:

        Transform();

        Transform(glm::vec3 position, glm::vec3 scale, glm::quat rotation);

        void setParent(std::weak_ptr<Transform> parent);

        void clearParent();

        const std::weak_ptr<Transform>& getParent() const;

        glm::mat4 getModelMatrix() const;

        glm::mat4 getWorldMatrix() const;

        void setLocalPosition(glm::vec3 position);
        
        void setLocalPositionByWorldPosition(glm::vec3 worldPosition);

        void adjustLocalPosition(glm::vec3 adjustment);

        glm::vec3 getLocalPosition() const;

        void setLocalScale(glm::vec3 scale);

        glm::vec3 getLocalScale() const;

        void setLocalRotation(glm::quat rotation);

        glm::quat getLocalRotation() const;

        glm::vec3 getLocalXAxis() const;

        glm::vec3 getLocalZAxis() const;

        glm::vec3 getLocalYAxis() const;

        glm::vec3 getWorldPosition() const;

        glm::vec3 getLocalForwardDirection() const;

    private:

        std::weak_ptr<Transform> parent;

        glm::vec3 localPosition { 0, 0, 0 };

        glm::vec3 localScale { 1 };

        glm::quat localRotation { 1, 0, 0, 0 };
    };

}