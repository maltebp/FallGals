#include "AnimationClip.hpp"

#include <cassert>

#include <sre/Mesh.hpp>

#include <Core/Transform.hpp>
#include "Model.hpp"
#include "ModelPose.hpp"

using namespace std;
using namespace sre;
using namespace fg::core;

using Builder = AnimationClip::Builder;


Builder::Builder(shared_ptr<const Model> model, shared_ptr<const ModelPose> basePose, float lengthSeconds) 
    :   clip(make_shared<AnimationClip>())
{
    assert(model != nullptr);
    assert(lengthSeconds > 0);

    clip->model = model;
    clip->lengthSeconds = lengthSeconds;

    addFrame(0.0, basePose);
}


Builder& Builder::addFrame(float frameTimeNormalized, shared_ptr<const ModelPose> modelPose) {

    assert(modelPose != nullptr);
    assert(modelPose->getModel() == clip->model);
    assert(frameTimeNormalized >= 0 && frameTimeNormalized <= 1.0f);
    
    AnimationClip::Frame frame;
    frame.timeNormalized = frameTimeNormalized;
    frame.pose = modelPose;

    clip->frames.push_back(frame);

    return *this;
}


Builder& Builder::enableLooping() {
    clip->isLoopingFlag = true;
    return *this;
}


shared_ptr<const AnimationClip> Builder::build() {
    assert(clip->frames.size() > 1);

    shared_ptr<const AnimationClip> builtClip = clip;
    clip = nullptr;

    return builtClip;
}


Transform AnimationClip::getPoseMeshTransformAtTime(float timeNormalized, const shared_ptr<const Mesh>& mesh) const {
    assert(timeNormalized >= 0);
    assert(timeNormalized <= 1.0f);

    auto [ frame1, frame2 ] = getFramesAtTime(timeNormalized);

    float frameLength = frame2->timeNormalized - frame1->timeNormalized;

    float relativeTime = timeNormalized - frame1->timeNormalized;

    float relativeTimeNormalized = relativeTime / frameLength;

    Transform transform1 = frame1->pose->getMeshTransform(mesh);
    Transform transform2 = frame2->pose->getMeshTransform(mesh);

    Transform poseTransform {
        mix(transform1.getLocalPosition(), transform2.getLocalPosition(), relativeTimeNormalized),
        mix(transform1.getLocalScale(), transform2.getLocalScale(), relativeTimeNormalized),
        slerp(transform1.getLocalRotation(), transform2.getLocalRotation(), relativeTimeNormalized)
    };

    return poseTransform;
}


tuple<const AnimationClip::Frame*, const AnimationClip::Frame*> AnimationClip::getFramesAtTime(
    float timeNormalized
) const {

    if( timeNormalized <= 0 ) {
        return make_tuple(&frames[0], &frames[1]);
    }
    
    if( timeNormalized >= 1 ) {
        return make_tuple(&frames[frames.size() -1 ], &frames[1]);
    }

    const Frame* previousFrame = &frames[0];
    const Frame* currentFrame = nullptr;

    for( int i = 1; i < frames.size(); i++) {

        currentFrame = &frames[i];

        if( timeNormalized >= previousFrame->timeNormalized  &&
            timeNormalized <= currentFrame->timeNormalized ) {
            break;
        }

        previousFrame = currentFrame;
    }

    return make_tuple(previousFrame, currentFrame);
}


float AnimationClip::getLengthSeconds() const {
    return lengthSeconds;
}

bool AnimationClip::isLooping() const {
    return isLoopingFlag;
}