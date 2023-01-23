#include <Core/Model/ModelInstance.hpp>

namespace fg::core {
    GameTime;
}

namespace fg::game {

    class RotatingObstacle : public core::ModelInstance {

    private:
        glm::vec3 _rotation;
        glm::vec3 rotationAxis;
        float rotationSpeed;


    public:
        RotatingObstacle();
        void setRotationAxis(glm::vec3 rotationAxis);
        void setRotationSpeed(float speed);

    protected :
        void onUpdate(const fg::core::GameTime& time) override;
    };
}