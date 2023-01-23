#pragma once

#include <memory>
#include <vector>
#include <map>

#include <btBulletDynamicsCommon.h>

#include <Core/Transform.hpp>
#include <Core/GameTime.hpp>
#include <Core/Entity.hpp>

namespace fg::core {

	struct CollisionPoint
	{
		btVector3& ptA;
		btVector3& ptB;
		btVector3 normalOnB;
		CollisionPoint() :ptA(btVector3(0, 0, 0)), ptB(btVector3(0, 0, 0)) {};
	};


	class Rigidbody : public Entity { 

		friend class Physics;
		
	public:
	
		// If mass == 0 the rigid body is static, otherwise it is dynamic.
		// To make it kinematic do Rigidbody->makeKinematic
		Rigidbody(std::weak_ptr<Transform> trans, float mass, std::shared_ptr<btCollisionShape> shape);

		float getMass();
		
		std::shared_ptr<btRigidBody> getBody();
		
		void makeKinematic();
		
		void makeStatic();
		
		void makeDynamic(float mass);
		
		bool isDynamic();
		
		bool isKinematic();
		
		void applyImpulse(glm::vec3 impulseVector);
		
		void applyForce(glm::vec3 forceVector);
		
		void clearForce();
		
		void setRestitution(float restitution);
		
		float getRestitution();
		
		void setFriction(float friction);
		
		float getFriction();
		
		void setAngularVelocity(glm::vec3 angularVelocity);
		
		glm::vec3 getAnguarVelocity();
		
		void setLinearVelocity(glm::vec3 linearVelocity);
		
		glm::vec3 getLinearVelocity();
		
		void syncTransform();

		std::weak_ptr<Transform> _Transform;

	protected:
		
		virtual void onEnterCollision(std::shared_ptr<Rigidbody> other, std::vector<CollisionPoint> collisionPoints) {};
		
		virtual void onExitCollision(std::shared_ptr<Rigidbody> other) {};
		
		virtual void onCollision(std::shared_ptr<Rigidbody> other, std::vector<CollisionPoint> collisionPoints) {};
		
		virtual void onPhysicsUpdate() { };

		std::shared_ptr<btRigidBody> body;
				
	private:

		void physicsUpdate();

		void collision(std::shared_ptr<Rigidbody> other, std::vector<CollisionPoint> collisionPoints);

		std::shared_ptr<btCollisionShape> shape;

		std::shared_ptr<btDefaultMotionState> defaultMotionState;

		float mass;

		std::map<std::weak_ptr<Rigidbody>, int, std::owner_less<>> collisionMap;

	};
}