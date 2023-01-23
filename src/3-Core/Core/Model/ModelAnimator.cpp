#include "ModelAnimator.hpp"

#include <cassert>

#include <glm/glm.hpp>

#include <Core/Transform.hpp>
#include <Core/GameTime.hpp>
#include "Model.hpp"
#include "ModelInstance.hpp"
#include "ModelPose.hpp"
#include "AnimationClip.hpp"

using namespace std;
using namespace glm;
using namespace sre;
using namespace fg::core;   


shared_ptr<ModelPose> makeModelPose(const weak_ptr<ModelInstance>& modelInstance) {
    shared_ptr<ModelInstance> modelInstanceShared = modelInstance.lock();
    if( modelInstanceShared == nullptr ) return nullptr;
    
    const shared_ptr<Model>& model = modelInstanceShared->getModel();
    if( model == nullptr ) return nullptr;

    int numMeshes = model->getMeshes().size();
    unordered_map<int, Transform> meshTransforms;

    for( int i = 0; i < numMeshes; i++ ) {
        meshTransforms[i] = Transform{};
    }

    return make_shared<ModelPose>(model, meshTransforms);
}


void setPoseTransformsFromPose(
    const shared_ptr<ModelPose>& targetPose, const shared_ptr<ModelPose>& sourcePose
) {
    assert(targetPose->getModel() == sourcePose->getModel());

    for( const MeshWithMaterial& meshWithMaterial : sourcePose->getModel()->getMeshes() ) {
        Transform& targetTransform = targetPose->getMeshTransform(meshWithMaterial.mesh);
        targetTransform = sourcePose->getMeshTransform(meshWithMaterial.mesh);
    }
}


void ModelAnimator::setModelInstance(weak_ptr<ModelInstance> modelInstance) {
    this->modelInstance = modelInstance; 
    currentPose = makeModelPose(modelInstance);
    transitionSourcePose = makeModelPose(modelInstance);
    animationClip = nullptr;
}


void ModelAnimator::setAnimationClip(shared_ptr<const AnimationClip> clip, float blendTimeSeconds) {
    assert(blendTimeSeconds >= 0);

    animationClip = clip;
    this->blendTimeSeconds = blendTimeSeconds;  
    currentBlendTimeNormalized = 0;
    timeNormalized = 0;
    
    if( blendTimeSeconds > 0 ) {
        setPoseTransformsFromPose(transitionSourcePose, currentPose);
    }
}


void ModelAnimator::onUpdate(const GameTime& gameTime) {
    if( animationClip == nullptr ) return; 

    shared_ptr<ModelInstance> modelInstancePtr = modelInstance.lock();
    if( modelInstancePtr == nullptr ) return;

    const shared_ptr<Model>& model = modelInstancePtr->getModel();
    if( model == nullptr ) return;

    if( blendTimeSeconds > 0 ) {
        currentBlendTimeNormalized += gameTime.frameTime / blendTimeSeconds;
        if( currentBlendTimeNormalized > 1.0f ) {
            blendTimeSeconds = 0;
        }
    }

    timeNormalized += gameTime.frameTime / animationClip->getLengthSeconds();

    if( timeNormalized > 1.0f ) {
        timeNormalized = animationClip->isLooping() ? fmod(timeNormalized, 1.0f) : 0;
    }   

    for( const MeshWithMaterial& meshWithMaterial : model->getMeshes() ) {
        Transform& poseTransform = currentPose->getMeshTransform(meshWithMaterial.mesh);
        Transform currentClipTransform = animationClip->getPoseMeshTransformAtTime(timeNormalized, meshWithMaterial.mesh);
        
        if( blendTimeSeconds == 0 ) {
            poseTransform = currentClipTransform;
        }
        else {
            const Transform& transitionSourceTransform = transitionSourcePose->getMeshTransform(meshWithMaterial.mesh);

            poseTransform.setLocalPosition(mix(
                transitionSourceTransform.getLocalPosition(), 
                currentClipTransform.getLocalPosition(), 
                currentBlendTimeNormalized
            ));

            poseTransform.setLocalScale(mix(
                transitionSourceTransform.getLocalScale(), 
                currentClipTransform.getLocalScale(), 
                currentBlendTimeNormalized
            ));

            poseTransform.setLocalRotation(slerp(
                transitionSourceTransform.getLocalRotation(),
                currentClipTransform.getLocalRotation(),
                currentBlendTimeNormalized
            ));
        }
    }

    modelInstancePtr->setPose(currentPose);
}


