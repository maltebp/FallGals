#pragma once

#include <memory>
#include <map>
#include <unordered_map>
#include <vector>

#include <Core/GameTime.hpp>
#include <Core/Rigidbody.hpp>
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"

using namespace std;

namespace fg::core {
	class Physics {
	public:
		Physics(Physics const&) = delete;
		void operator=(Physics const&) = delete;
		static Physics* getInstance();
		static void initWorld(glm::vec3 gravity);
		static void deleteWorld();
		static btVector3 glmToBtVec3(glm::vec3 vector);
		static glm::vec3 btToGlmVec3(btVector3 vector);
		void update(float deltaTime);
		void setGravity(glm::vec3 grav);
		void addRigidBody(std::weak_ptr<Rigidbody> body);
		const btCollisionWorld::AllHitsRayResultCallback rayCastAll(glm::vec3 from, glm::vec3 to);
		const btCollisionWorld::ClosestRayResultCallback rayCastFirst(glm::vec3 from, glm::vec3 to);
	private:
		btDefaultCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* world;

		std::map<btRigidBody*, std::weak_ptr<Rigidbody>> Rigidbodies;

		// This exists only to keep the shared ptr to the rigid body alive
		std::unordered_map<btRigidBody*, std::shared_ptr<btRigidBody>> bulletRigidBodySharedPointers;

		void syncWorldTransform();
		void UpdateWorldCollisions();
		void UpdatePhysiscs();
		void syncKinematicObject(shared_ptr<Rigidbody> body, shared_ptr<Transform> trans);
		void removeDeletedRigidbodies();
		~Physics();
		Physics(glm::vec3 gravity);
		friend void TickCallback(btDynamicsWorld* world, btScalar ts);
	};
}
