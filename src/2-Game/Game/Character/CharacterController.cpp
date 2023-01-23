#include "CharacterController.hpp"

#include <SDL_events.h>

#include <Utility/Math.hpp>
#include <Core/Transform.hpp>
#include <Core/Camera.hpp>
#include <Game/Character/Character.hpp>

using namespace std;
using namespace glm;
using namespace fg::core;
using namespace fg::utility;
using namespace fg::game;


CharacterController::CharacterController(Character* character, Camera* camera)
	: 	character(character),
		camera(camera)
{ }


bool CharacterController::onKeyEvent(SDL_KeyboardEvent& keyboardEvent)
{
	bool keyIsDown = keyboardEvent.type == SDL_KEYDOWN;

	if (keyboardEvent.type == SDL_KEYUP && keyboardEvent.keysym.sym == SDLK_SPACE && character->isGrounded()) {
		character->applyImpulse(vec3(0, 5, 0));
		return true;
	}

	switch (keyboardEvent.keysym.sym)
	{
		case SDLK_UP:
		case SDLK_w:
			forward = keyIsDown;
			return true;
		case SDLK_LEFT:
		case SDLK_a:
			left = keyIsDown;
			return true;
		case SDLK_RIGHT:
		case SDLK_d:
			right = keyIsDown;
			return true;
		case SDLK_DOWN:
		case SDLK_s:
			back = keyIsDown;
			return true;
	}
	return false;
}


void CharacterController::onUpdate(const fg::core::GameTime& time) {

	Transform& transform = *character->transform;

	if( character->isGrounded() ) {
		vec3 dir = {0,0,0};
		if (forward)
			dir += transform.getLocalForwardDirection();
		if (back)
			dir -= transform.getLocalForwardDirection();
		if (left)
			playerYRotation += 100 * time.frameTime;
		if (right)
			playerYRotation -= 100 * time.frameTime;

		dir = length(dir) != 0 ? normalize(dir) : dir;
		character->applyForce(dir * walkingSpeed * time.frameTime);
		quat playerRotationQuat = quat(radians(vec3{ 0.0f, playerYRotation, 0.0f }));
		transform.setLocalRotation(playerRotationQuat);
	}

	vec3 vel = character->getLinearVelocity();

	if (vel.y < -0.1f)
		character->setCurrentAnimation(CharacterAnimation::FALL);
	else if (vel.y > 0.1f)
		character->setCurrentAnimation(CharacterAnimation::JUMP);
	else if (vel.x > 0.1f || vel.x < -0.1f || vel.z > 0.1f || vel.z < -0.1f)
		character->setCurrentAnimation(CharacterAnimation::RUN);
	else
		character->setCurrentAnimation(CharacterAnimation::IDLE);

	// Reset position when character falls out of the map
	if( transform.getLocalPosition().y < -10 ) {
		character->setLinearVelocity(vec3(0, 0, 0));
		transform.setLocalPosition(vec3(0, 0, 0));
	}
}

bool CharacterController::onMouseMotionEvent(SDL_MouseMotionEvent& mouseMotionEvent) {

	return false;
	float yMovement = mouseMotionEvent.yrel;
	cameraXRotation += -yMovement * rotationSpeed;
	cameraXRotation = clamp(cameraXRotation, -maxXRotation, maxXRotation);
	Math::wrapDegrees(cameraXRotation);

	float xMovement = mouseMotionEvent.xrel;
	playerYRotation += -xMovement * rotationSpeed;
	Math::wrapDegrees(playerYRotation);

	quat camRotationQuat = quat(radians(vec3{ cameraXRotation, 0.0f, 0.0f }));
	quat playerRotationQuat = quat(radians(vec3{ 0.0f, playerYRotation, 0.0f }));
	character->transform->setLocalRotation(playerRotationQuat);
	camera->getTransform()->setLocalRotation(camRotationQuat);
}