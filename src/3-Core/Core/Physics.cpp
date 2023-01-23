#include "Physics.hpp"

#include <cassert>
#include <stdio.h>

#include <SDL_opengl.h>

using namespace std;
using namespace fg::core;

Physics::Physics(glm::vec3 gravity) 
{
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfig);
	world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	world->setInternalTickCallback(TickCallback);
}

fg::core::Physics::~Physics()
{
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		world->removeCollisionObject(obj);
	}

	delete world;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfig;
	Rigidbodies.clear();
}

Physics* _physics;

Physics* fg::core::Physics::getInstance()
{
	if (_physics == nullptr) throw exception("Physics not initialised");
	return _physics;
}

void fg::core::Physics::initWorld(glm::vec3 gravity)
{
	if (_physics != nullptr)
		deleteWorld();
	_physics = new Physics(gravity);
}

void fg::core::Physics::deleteWorld()
{
	delete _physics;
}

void fg::core::Physics::update(float deltaTime) {
	removeDeletedRigidbodies();
	syncWorldTransform();
	world->stepSimulation(deltaTime, 10);
}

void fg::core::TickCallback(btDynamicsWorld* world, btScalar ts) {
	_physics->UpdateWorldCollisions();
	_physics->UpdatePhysiscs();
}

btVector3 fg::core::Physics::glmToBtVec3(glm::vec3 vector)
{
	return btVector3(vector.x, vector.y, vector.z);
}

glm::vec3 fg::core::Physics::btToGlmVec3(btVector3 vector)
{
	return glm::vec3(vector.x(), vector.y(), vector.z());
}


void Physics::UpdateWorldCollisions() {
	
	int manifolds = world->getDispatcher()->getNumManifolds();

	for (int i = 0; i < manifolds; i++) {
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		const btRigidBody* bulletRigidBodyA = btRigidBody::upcast(contactManifold->getBody0());
		const btRigidBody* bulletRigidBodyB = btRigidBody::upcast(contactManifold->getBody1());

		shared_ptr<Rigidbody> rigidBody1 = Rigidbodies[const_cast<btRigidBody*>(bulletRigidBodyA)].lock();
		assert(rigidBody1 != nullptr);

		shared_ptr<Rigidbody> rigidBody2 = Rigidbodies[const_cast<btRigidBody*>(bulletRigidBodyB)].lock();
		assert(rigidBody2 != nullptr);

		int contacts = contactManifold->getNumContacts();
		vector<CollisionPoint> collisionPoints(contacts);
		for (int j = 0; j < contacts; j++)
		{
			CollisionPoint cPoint;
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance() < 0.f)
			{
				cPoint.ptA = pt.getPositionWorldOnA();
				cPoint.ptB = pt.getPositionWorldOnB();
				cPoint.normalOnB = pt.m_normalWorldOnB;
			}
			collisionPoints.push_back(cPoint);
		}

		rigidBody1->collision(rigidBody2, collisionPoints);
		rigidBody2->collision(rigidBody1, collisionPoints);
	}
}

void fg::core::Physics::UpdatePhysiscs()
{
	for (auto const& [bulletRigidbody, rigidBody] : Rigidbodies) {
		shared_ptr<Rigidbody> rigidBodyShared = rigidBody.lock();
		assert(rigidBodyShared != nullptr);
		rigidBodyShared->physicsUpdate();
	}
	_physics->syncWorldTransform();
}

void Physics::syncKinematicObject(shared_ptr<Rigidbody> body, shared_ptr<Transform> trans) {
	btTransform wtrans;
	body->getBody()->getMotionState()->getWorldTransform(wtrans);
	auto pos = trans->getWorldPosition();
	auto rot = trans->getLocalRotation();
	wtrans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	btQuaternion quart = wtrans.getRotation();
	auto euler = glm::eulerAngles(rot);
	quart.setEulerZYX(euler.z, euler.y, euler.x);
	wtrans.setRotation(quart);
	body->getBody()->getMotionState()->setWorldTransform(wtrans);
}

void Physics::syncWorldTransform() {
	for( auto const& [bulletRigidBody, rigidBody] : Rigidbodies ) {

		shared_ptr<Rigidbody> rigidBodyShared = rigidBody.lock();
		assert(rigidBodyShared != nullptr);

		shared_ptr<Transform> rigidBodyTransformShared = rigidBodyShared->_Transform.lock();
		if( rigidBodyTransformShared == nullptr ) continue;
		
		if( bulletRigidBody->isKinematicObject() ) { 
			syncKinematicObject(rigidBodyShared, rigidBodyTransformShared); 
			continue; 
		}

		btTransform wtrans = bulletRigidBody->getWorldTransform();
		auto pos = rigidBodyTransformShared->getWorldPosition();
		auto rot = rigidBodyTransformShared->getLocalRotation();
		wtrans.setOrigin(btVector3(pos.x, pos.y, pos.z));
		btQuaternion quart = wtrans.getRotation();
		auto euler = glm::eulerAngles(rot);
		quart.setEulerZYX(euler.z, euler.y, euler.x);
		wtrans.setRotation(quart);
		rigidBodyShared->getBody()->setWorldTransform(wtrans);
	}
}

void Physics::setGravity(glm::vec3 grav)
{
	world->setGravity(btVector3(grav.x, grav.y, grav.z));
}

void fg::core::Physics::addRigidBody(weak_ptr<Rigidbody> body)
{
	shared_ptr<Rigidbody> bodyShared = body.lock();
	assert(bodyShared != nullptr);

	shared_ptr<btRigidBody> bulletRigidBody = bodyShared->getBody();
	assert(bulletRigidBody != nullptr);

	bulletRigidBodySharedPointers[bulletRigidBody.get()] = bulletRigidBody;
	Rigidbodies[bulletRigidBody.get()] = body;
	world->addRigidBody(bulletRigidBody.get());
}

const btCollisionWorld::AllHitsRayResultCallback fg::core::Physics::rayCastAll(glm::vec3 from, glm::vec3 to)
{
	auto bFrom = glmToBtVec3(from);
	auto bTo = glmToBtVec3(to);

	btCollisionWorld::AllHitsRayResultCallback allResults(bFrom, bTo);
	allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
	allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
	world->rayTest(bFrom, bTo, allResults);
	return allResults;
}

const btCollisionWorld::ClosestRayResultCallback fg::core::Physics::rayCastFirst(glm::vec3 from, glm::vec3 to)
{
	auto bFrom = glmToBtVec3(from);
	auto bTo = glmToBtVec3(to);

	btCollisionWorld::ClosestRayResultCallback closestResults(bFrom, bTo);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;

	world->rayTest(bFrom, bTo, closestResults);
	return closestResults;
}


void Physics::removeDeletedRigidbodies() {

	for( auto it = Rigidbodies.begin(); it != Rigidbodies.end(); ) {
		weak_ptr<Rigidbody>& rigidBody = it->second;

		if( rigidBody.expired() ) {
			btRigidBody* bulletRigidBody = it->first;
			world->removeRigidBody(bulletRigidBody);
			bulletRigidBodySharedPointers.erase(bulletRigidBody);
			it = Rigidbodies.erase(it);
		}
		else {
			it++;
		}
	}
}