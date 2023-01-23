#pragma once

#include <memory>

namespace fg::core {
    class Model;
    class ModelPose;
    class AnimationClip;
}

namespace fg::game {

    class CharacterAssets {
    public:

        CharacterAssets();

        const std::shared_ptr<core::Model> model;
        
        const std::shared_ptr<const core::AnimationClip> idleAnimation;

        const std::shared_ptr<const core::AnimationClip> runAnimation;

        const std::shared_ptr<const core::AnimationClip> walkAnimation;

        const std::shared_ptr<const core::AnimationClip> jumpAnimation;

        const std::shared_ptr<const core::AnimationClip> fallAnimation;

    };

}