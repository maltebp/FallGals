#include "Character.hpp"

#include <Utility/CollisionShapes.hpp>
#include <Core/Transform.hpp>
#include <Core/Camera.hpp>
#include <Core/Model/ModelPose.hpp>
#include <Core/Model/ModelInstance.hpp>
#include <Core/Model/ModelAnimator.hpp>
#include <Core/Model/AnimationClip.hpp>
#include <Game/Character/CharacterAssets.hpp>
#include <Game/Character/CharacterController.hpp>
#include <Game/Map/FinishLine.hpp>

using namespace std;
using namespace glm;
using namespace fg::utility;
using namespace fg::game;
using namespace fg::core;


Character::Character(shared_ptr<const CharacterAssets> assets, function<void()> goalReachedCallback) 
    :   Rigidbody(
            weak_ptr<Transform>{},
            1,
            CollisionShapes::GetBoxShape(0.25f, 0.55f, 0.1f)
        ),
        assets(assets),
        goalReachedCallback(goalReachedCallback),
        transform(make_shared<Transform>()),
        camera(createChild<Camera>()),
        modelInstance(createChild<ModelInstance>()),
        animator(createChild<ModelAnimator>()),
        controller(createChild<CharacterController>(this, camera.get()))
{ 
	assert(goalReachedCallback != nullptr);

    camera->getTransform()->setParent(transform);
    camera->getTransform()->setLocalPosition(vec3{0, 0.5f, 3.0f});

    modelInstance->transform->setLocalPosition({ 0, -0.65f, 0 });
    modelInstance->transform->setParent(transform);
    modelInstance->setModel(assets->model);
    animator->setModelInstance(modelInstance);

    _Transform = transform;
    body->setAngularFactor(btVector3(0.f, 0.0f, 0.0f));
    body->setFriction(1.f);

    setCurrentAnimation(CharacterAnimation::IDLE, false);
}


void Character::setCurrentAnimation(CharacterAnimation animation, bool blend) {
    if( currentAnimation == animation ) return;

    currentAnimation = animation;

    switch(animation) {
        case CharacterAnimation::IDLE:
            animator->setAnimationClip(assets->idleAnimation, 0.25f * blend);
            break;
        case CharacterAnimation::RUN:
            animator->setAnimationClip(assets->runAnimation, 0.25f * blend);
            break;
        case CharacterAnimation::JUMP:
            animator->setAnimationClip(assets->jumpAnimation, 0.25f * blend);
            break;
        case CharacterAnimation::FALL:
            animator->setAnimationClip(assets->fallAnimation, 0.25f * blend);
            break;
    }
}


CharacterAnimation Character::getCurrentAnimation() const {
    return currentAnimation;
}


bool Character::isGrounded() {
	return getLinearVelocity().y < 0.1f && getLinearVelocity().y > -0.1f;
}


void Character::onCollision(shared_ptr<Rigidbody> other, vector<CollisionPoint> collisionPoints) {
	if( dynamic_pointer_cast<FinishLine>(other) != nullptr ) {
		goalReachedCallback();		
	}
}