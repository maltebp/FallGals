#pragma once

#include <memory>
#include <vector>
#include <tuple>

namespace sre {
    class Mesh;
}

namespace fg::core {

    class Transform;
    class Model;
    class ModelPose;


    class AnimationClip {
    public:

        class Builder {
        public:

            Builder(std::shared_ptr<const Model> model, std::shared_ptr<const ModelPose> basePose, float lengthSeconds);

            Builder& addFrame(float frameTimeNormalized, std::shared_ptr<const ModelPose> pose);

            Builder& enableLooping();

            std::shared_ptr<const AnimationClip> build();

        private:

            std::shared_ptr<AnimationClip> clip;

        };

        Transform getPoseMeshTransformAtTime(float timeNormalized, const std::shared_ptr<const sre::Mesh>& mesh) const;

        float getLengthSeconds() const;        

        bool isLooping() const;

    private:

        struct Frame {
            float timeNormalized;
            std::shared_ptr<const ModelPose> pose;
        };

        std::tuple<const Frame*, const Frame*> getFramesAtTime(float timeNormalized) const;

        std::shared_ptr<const Model> model;

        std::vector<Frame> frames;

        float lengthSeconds;

        bool isLoopingFlag;

    };
}