#include "RotatingObstacle.hpp"

#include <Core/GameTime.hpp>
using namespace fg::core;
using namespace fg::game;


fg::game::RotatingObstacle::RotatingObstacle()
{
	this->_rotation = glm::eulerAngles(transform->getLocalRotation());
	this->rotationAxis = glm::vec3(0, 1, 0);
	this->rotationSpeed = 1;
}

void fg::game::RotatingObstacle::setRotationAxis(glm::vec3 rotationAxis)
{
	this->rotationAxis = rotationAxis;
}

void fg::game::RotatingObstacle::setRotationSpeed(float speed)
{
	this->rotationSpeed = speed;
}

void RotatingObstacle::onUpdate(const GameTime& time) {
	_rotation = _rotation + (rotationAxis * rotationSpeed * time.frameTime);
	transform->setLocalRotation(_rotation);
}