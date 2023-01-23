#pragma once 

#include <memory>

#include <Core/Rigidbody.hpp>

namespace fg::core {
    class GameTime;
    class Transform;
}

namespace fg::game {

    class CannonBall : public core::Rigidbody {
    public:

        CannonBall(glm::vec3 position, glm::vec3 initialImpulse);

    protected:

        void onUpdate(const core::GameTime& time) override;

    private:

        const std::shared_ptr<core::Transform> transform;

    };

}