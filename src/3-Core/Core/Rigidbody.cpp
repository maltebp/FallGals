#include "Rigidbody.hpp"

#include <Core/Physics.hpp>

using namespace std;
using namespace glm;
using namespace fg::core;


Rigidbody::Rigidbody(weak_ptr<Transform> trans, float mass, std::shared_ptr<btCollisionShape> shape) 
	:	shape(shape)
{
	btTransform startTransform;
	startTransform.setIdentity();

	this->mass = mass;
	_Transform = trans;
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f) {
		shape->calculateLocalInertia(mass, localInertia);
	}

	shared_ptr<Transform> transformShared = trans.lock();
	if( transformShared != nullptr ) {
		auto worldPos = transformShared->getWorldPosition();
		auto rot = transformShared->getLocalRotation();
		startTransform.setOrigin(Physics::glmToBtVec3(worldPos));
		startTransform.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));	
	}
	
	defaultMotionState = make_shared<btDefaultMotionState>(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo {
		mass, defaultMotionState.get(), shape.get(), localInertia
	};
	body = make_shared<btRigidBody>(rbInfo);
	if( mass != 0.0f ) {
		body->setActivationState(DISABLE_DEACTIVATION);
	} 
	body->setFriction(1.f);
	body->setRestitution(0.1f);
}


float Rigidbody::getMass() {
	return mass;
}


shared_ptr<btRigidBody> Rigidbody::getBody() {
	return body;
}


void Rigidbody::makeKinematic() {
	if (body->isStaticOrKinematicObject() && !body->isKinematicObject()) {
		body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
		body->activate();
	}
	else {
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
		body->activate();
		btVector3 localInertia(0, 0, 0);
		mass = 0.f;
		body->setMassProps(mass, localInertia);
	}
}


void Rigidbody::makeStatic() {
	if (body->isStaticOrKinematicObject() && body->isKinematicObject()) {
		body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		body->setActivationState(WANTS_DEACTIVATION);
	}
	else {
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		body->setActivationState(WANTS_DEACTIVATION);
		btVector3 localInertia(0, 0, 0);
		mass = 0.f;
		body->setMassProps(mass, localInertia);
	}
}


void Rigidbody::makeDynamic(float mass) {
	body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
	body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
	this->mass = mass;
	btVector3 localInertia(0, 0, 0);
	if (mass != 0.f)
		shape->calculateLocalInertia(mass, localInertia);
	body->setMassProps(mass, localInertia);
}


bool Rigidbody::isDynamic() {
	return !body->isStaticOrKinematicObject();
}


bool Rigidbody::isKinematic() {
	return body->isKinematicObject();
}


void Rigidbody::physicsUpdate() {
	syncTransform();
	
	for( auto it = collisionMap.begin(); it != collisionMap.end(); ) {
		shared_ptr<Rigidbody> collidedBodyShared = it->first.lock();

		if( collidedBodyShared == nullptr ) {
			// onExitCollision is not called, because we have not pointer to
			// the object
			it = collisionMap.erase(it);
			continue;
		}
		
		it->second = it->second + 1;
		if( it->second > 1 ) {
			onExitCollision(collidedBodyShared);
			it = collisionMap.erase(it);
			continue;
		}

		it++;
	}
	onPhysicsUpdate();
}

void Rigidbody::syncTransform() {
	if (!body->getMotionState()) return;

	shared_ptr<Transform> transformShared = _Transform.lock();
	if( transformShared == nullptr ) return; 

	auto wt = body->getWorldTransform();
	const auto& pos = wt.getOrigin();
	const auto& rot = wt.getRotation();
	transformShared->setLocalPositionByWorldPosition(Physics::btToGlmVec3(pos));
	float x, y, z;
	rot.getEulerZYX(z, y, x);
	auto glmRot = quat(vec3(x, y, z));
	transformShared->setLocalRotation(glmRot);
}


void Rigidbody::applyImpulse(vec3 impulseVector) {
	body->applyCentralImpulse(Physics::glmToBtVec3(impulseVector));
}


void Rigidbody::applyForce(vec3 forceVector) {
	body->applyCentralForce(Physics::glmToBtVec3(forceVector));
}


void Rigidbody::clearForce() {
	body->clearForces();
}


void Rigidbody::setRestitution(float restitution) {
	body->setRestitution(restitution);
}


float Rigidbody::getRestitution() {
	return body->getRestitution();
}


void Rigidbody::setFriction(float friction) {
	body->setFriction(friction);
}


float Rigidbody::getFriction() {
	return body->getFriction();
}


void Rigidbody::setAngularVelocity(vec3 angularVelocity) {
	body->setAngularVelocity(Physics::glmToBtVec3(angularVelocity));
}


vec3 Rigidbody::getAnguarVelocity() {
	return  Physics::btToGlmVec3(body->getAngularVelocity());
}


void Rigidbody::setLinearVelocity(vec3 linearVelocity) {
	body->setLinearVelocity(Physics::glmToBtVec3(linearVelocity));
}


vec3 Rigidbody::getLinearVelocity() {
	return Physics::btToGlmVec3(body->getLinearVelocity());
}


void Rigidbody::collision(shared_ptr<Rigidbody> other, vector<CollisionPoint> collisionPoints) {
	if (collisionMap.count(other) == 0) {
		collisionMap.insert({ other, 0 });
		onEnterCollision(other, collisionPoints);
	}
	else {
		collisionMap[other] = collisionMap[other] - 1;
	}

	onCollision(other, collisionPoints);
}
