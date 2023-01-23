#pragma once

#include <Core/Model/ModelInstance.hpp>

namespace fg::core {
    class GameTime;
}

namespace fg::game {

    class CanonObstacle : public core::ModelInstance {
    public:
        
        CanonObstacle(glm::vec3 firingAngle);

    protected:
    
        void onUpdate(const fg::core::GameTime& time) override;

    private:

        glm::vec3 firingAngle;

        float firingSpeed;

        float timeSinceLastFire;

    };
}