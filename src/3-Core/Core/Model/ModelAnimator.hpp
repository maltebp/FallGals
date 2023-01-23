#pragma once

#include <memory>

#include <Core/Entity.hpp>

namespace fg::core {

    class GameTime;
    class ModelInstance;
    class ModelPose;
    class AnimationClip;

    
    class ModelAnimator : public Entity {
    public:

        ModelAnimator() { }

        void setModelInstance(std::weak_ptr<ModelInstance> modelInstance);

        void setAnimationClip(std::shared_ptr<const AnimationClip> animationClip, float blendTimeSeconds = 0.0f);

    protected:

        void onUpdate(const GameTime& gameTime) override;

    private:

        std::weak_ptr<ModelInstance> modelInstance;

        std::shared_ptr<const AnimationClip> animationClip;

        std::shared_ptr<ModelPose> currentPose;

        float timeNormalized = 0.0f;

        float currentBlendTimeNormalized = 0.0f;

        float blendTimeSeconds = 0.0f;

        std::shared_ptr<ModelPose> transitionSourcePose;

    };
}