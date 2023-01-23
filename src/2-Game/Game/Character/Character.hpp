#pragma once

#include <memory>
#include <vector>
#include <functional>

#include <Core/Rigidbody.hpp>

namespace fg::core {
    class Transform;
    class ModelInstance;
    class ModelAnimator;
    class AnimationClip;
    class Camera;
}

namespace fg::game {

    class CharacterAssets;
    class CharacterController;


    enum class CharacterAnimation {
        IDLE,
        RUN,
        JUMP,
        FALL
    };

    
    class Character : public core::Rigidbody {
    public:

        Character(std::shared_ptr<const CharacterAssets> assets, std::function<void()> goalReachedCallback);

        void setCurrentAnimation(CharacterAnimation animation, bool blend = true);

        CharacterAnimation getCurrentAnimation() const;

        bool isGrounded();

        const std::shared_ptr<core::Transform> transform;
        
        const std::shared_ptr<core::Camera> camera;

    private:

        void onCollision(std::shared_ptr<core::Rigidbody> other, std::vector<core::CollisionPoint> collisionPoints) override;

        const std::shared_ptr<const CharacterAssets> assets;

        const std::shared_ptr<core::ModelInstance> modelInstance;

        const std::shared_ptr<core::ModelAnimator> animator;

        const std::shared_ptr<CharacterController> controller;

        const std::function<void()> goalReachedCallback;

        CharacterAnimation currentAnimation;

    };

}