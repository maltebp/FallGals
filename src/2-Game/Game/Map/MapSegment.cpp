#include "MapSegment.hpp"
#include <Core/Model/Model.hpp>
#include <Utility/MeshShapes.hpp>
#include <Utility/CollisionShapes.hpp>
#include <Game/Map/RotatingObstacle.hpp>
#include <Game/Map/CanonObstacle.hpp>
#include <Game/Map/FinishLine.hpp>
#include <Core/Rigidbody.hpp>
#include <Core/Physics.hpp>

using namespace std;
using namespace glm;
using namespace fg::core;
using namespace fg::game;
using namespace fg::utility;

shared_ptr<Model> createBoxModel2(float xsize, float ysize, float zsize, sre::Color color) {

	shared_ptr<sre::Mesh> mesh = MeshShapes::GetBoxShape(xsize, ysize, zsize);

	shared_ptr<sre::Material> material = sre::Shader::getStandardBlinnPhong()->createMaterial();
	material->setColor(color);
	material->setSpecularity(sre::Color(1, 1, 1, 50));

	shared_ptr<Model> model = make_shared<Model>();
	model->addMesh(mesh, material);

	return model;
}

shared_ptr<Model> createSphereModel(float size, sre::Color color) {

	shared_ptr<sre::Mesh> mesh = sre::Mesh::create().withSphere(16, 32, 0.5f).build();

	shared_ptr<sre::Material> material = sre::Shader::getStandardBlinnPhong()->createMaterial();
	material->setColor(color);
	material->setSpecularity(sre::Color(1, 1, 1, 50));

	shared_ptr<Model> model = make_shared<Model>();
	model->addMesh(mesh, material);

	return model;
}

void MapSegment::addMapSegment(glm::vec3 positionOffset, EntityGroup& entity)
{
	shared_ptr<Model> obstacleModel = createBoxModel2(2.0f, 1.0f, 1.0f, sre::Color{ 0.3f, 0.7f, 0.3f });
	shared_ptr<RotatingObstacle> obstacle = entity.createChild<RotatingObstacle>();
	obstacle->setRotationSpeed(0.3f);
	obstacle->setModel(obstacleModel);
	obstacle->transform->setLocalPosition(glm::vec3(2.2f, -5, 0.0f) + positionOffset);
	shared_ptr<Rigidbody> obstacleRigidBody = entity.createChild<Rigidbody>(
		obstacle->transform,
		1,
		CollisionShapes::GetBoxShape(2, 1, 1)
		);
	obstacleRigidBody->makeKinematic();
	Physics::getInstance()->addRigidBody(obstacleRigidBody);
}

void BlankSegment::addMapSegment(glm::vec3 positionOffset, EntityGroup& entity) {
	shared_ptr<Model> groundModel = createBoxModel2(5.0f, 0.5f, 5.0f, sre::Color{ 0.7f, 0.5f, 1.0f });
	shared_ptr<ModelInstance> ground = entity.createChild<ModelInstance>();
	ground->setModel(groundModel);
	ground->transform->setLocalPosition(glm::vec3(0, -5, 0) + positionOffset);

	shared_ptr<Rigidbody> groundRigidBody = entity.createChild<Rigidbody>(
		ground->transform,
		0,
		CollisionShapes::GetBoxShape(5.0f, 0.5f, 5.0f)
		);
	Physics::getInstance()->addRigidBody(groundRigidBody);
}

void RotatingPlatformsSegment::addMapSegment(glm::vec3 positionOffset, EntityGroup& entity)
{
	// Rotating platform 1
	shared_ptr<Model> obstacleModel1 = createBoxModel2(2.0f, 0.5f, 1.0f, sre::Color{ 0.3f, 0.7f, 0.3f });
	shared_ptr<RotatingObstacle> obstacle1 = entity.createChild<RotatingObstacle>();
	obstacle1->setRotationSpeed(0.3f);
	obstacle1->setModel(obstacleModel1);
	obstacle1->transform->setLocalPosition(glm::vec3(-2.5f, -5, 0.0f) + positionOffset);
	shared_ptr<Rigidbody> obstacleRigidBody1 = entity.createChild<Rigidbody>(
		obstacle1->transform,
		0,
		CollisionShapes::GetBoxShape(2.0f, 0.5f, 1.0f)
		);
	obstacleRigidBody1->makeKinematic();
	Physics::getInstance()->addRigidBody(obstacleRigidBody1);
	

	// Rotating platform 2
	shared_ptr<Model> obstacleModel2 = createBoxModel2(2.0f, 0.5f, 1.0f, sre::Color{ 0.3f, 0.7f, 0.3f });
	shared_ptr<RotatingObstacle> obstacle2 = entity.createChild<RotatingObstacle>();
	obstacle2->setRotationSpeed(0.3f);
	obstacle2->setModel(obstacleModel2);
	obstacle2->transform->setLocalPosition(glm::vec3(2.5f, -5, 0.0f) + positionOffset);
	shared_ptr<Rigidbody> obstacleRigidBody2 = entity.createChild<Rigidbody>(
		obstacle2->transform,
		0,
		CollisionShapes::GetBoxShape(2.0f, 0.5f, 1.0f)
		);
	obstacleRigidBody2->makeKinematic();
	Physics::getInstance()->addRigidBody(obstacleRigidBody2);
}

void BallPitSegment::addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup& entity)
{
	BlankSegment::addMapSegment(positionOffset, entity);

	for (int i = 0; i < 10; i++) {
		shared_ptr<Model> ballModel = createSphereModel(0.5f, sre::Color{ (rand()) / static_cast <float> (RAND_MAX), (rand()) / static_cast <float> (RAND_MAX), (rand()) / static_cast <float> (RAND_MAX) });
		shared_ptr<ModelInstance> ball = entity.createChild<ModelInstance>();
		ball->setModel(ballModel);
		ball->transform->setLocalPosition(glm::vec3(-5.0f + i, -2 + i, -5.0f + i) + positionOffset);
		shared_ptr<Rigidbody> ballRigidBody = entity.createChild<Rigidbody>(
			ball->transform,
			1,
			CollisionShapes::GetSphereShape(0.5f)
			);
		Physics::getInstance()->addRigidBody(ballRigidBody);
	}

	shared_ptr<CanonObstacle> canon1 = entity.createChild<CanonObstacle>(glm::vec3(0,1,1));
	canon1->transform->setLocalPosition(glm::vec3(0, -2, -5.0f) + positionOffset);

	shared_ptr<CanonObstacle> canon2 = entity.createChild<CanonObstacle>(glm::vec3(0, 1, -1));
	canon2->transform->setLocalPosition(glm::vec3(0, -2, 5.0f) + positionOffset);
}

void RevolvingDoorSegment::addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup& entity)
{
	BlankSegment::addMapSegment(positionOffset, entity);

	shared_ptr<Model> obstacleModel1 = createBoxModel2(6.0f, 0.5f, 0.2f, sre::Color{ 0.3f, 0.7f, 0.3f });
	shared_ptr<RotatingObstacle> obstacle1 = entity.createChild<RotatingObstacle>();
	obstacle1->setRotationSpeed(1.5f);
	obstacle1->setModel(obstacleModel1);
	obstacle1->transform->setLocalPosition(glm::vec3(0.0f, -4, 0.0f) + positionOffset);
	shared_ptr<Rigidbody> obstacleRigidBody1 = entity.createChild<Rigidbody>(
		obstacle1->transform,
		0,
		CollisionShapes::GetBoxShape(6.0f, 0.5f, 0.2f)
		);
	obstacleRigidBody1->makeKinematic();
	Physics::getInstance()->addRigidBody(obstacleRigidBody1);
}

void fg::game::DangerBridgeSegment::addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup& entity)
{
	// Bridge
	shared_ptr<Model> groundModel = createBoxModel2(5.0f, 0.5f, 1.5f, sre::Color{ 0.7f, 0.5f, 1.0f });
	shared_ptr<ModelInstance> ground = entity.createChild<ModelInstance>();
	ground->setModel(groundModel);
	ground->transform->setLocalPosition(glm::vec3(0, -5, 0) + positionOffset);

	shared_ptr<Rigidbody> groundRigidBody = entity.createChild<Rigidbody>(
		ground->transform,
		0,
		CollisionShapes::GetBoxShape(5.0f, 0.5f, 1.5f)
		);
	Physics::getInstance()->addRigidBody(groundRigidBody);

	//Obstacles
	shared_ptr<Model> obstacleModel1 = createBoxModel2(0.5f, 2.5f, 0.5f, sre::Color{ 0.3f, 0.7f, 0.3f });
	shared_ptr<RotatingObstacle> obstacle1 = entity.createChild<RotatingObstacle>();
	obstacle1->setRotationSpeed(2.0f);
	obstacle1->setRotationAxis(vec3(1, 0, 0));
	obstacle1->setModel(obstacleModel1);
	obstacle1->transform->setLocalPosition(glm::vec3(0, -2, 0.0f) + positionOffset);
	shared_ptr<Rigidbody> obstacleRigidBody1 = entity.createChild<Rigidbody>(
		obstacle1->transform,
		0,
		CollisionShapes::GetBoxShape(0.5f, 2.5f, 0.5f)
		);
	obstacleRigidBody1->makeKinematic();
	Physics::getInstance()->addRigidBody(obstacleRigidBody1);

	shared_ptr<Model> obstacleModel2 = createBoxModel2(0.5f, 2.5f, 0.5f, sre::Color{ 0.3f, 0.7f, 0.3f });
	shared_ptr<RotatingObstacle> obstacle2 = entity.createChild<RotatingObstacle>();
	obstacle2->setRotationSpeed(1.7f);
	obstacle2->setRotationAxis(vec3(1, 0, 0));
	obstacle2->setModel(obstacleModel2);
	obstacle2->transform->setLocalPosition(glm::vec3(0, -2, 0.0f) + positionOffset);
	shared_ptr<Rigidbody> obstacleRigidBody2 = entity.createChild<Rigidbody>(
		obstacle2->transform,
		0,
		CollisionShapes::GetBoxShape(0.5f, 2.5f, 0.5f)
		);
	obstacleRigidBody2->makeKinematic();
	Physics::getInstance()->addRigidBody(obstacleRigidBody2);


}

void FinishLineSegment::addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup& entity)
{
	shared_ptr<Model> groundModel = createBoxModel2(2.5f, 0.5f, 5.0f, sre::Color{ 0.7f, 0.5f, 1.0f });
	shared_ptr<ModelInstance> ground = entity.createChild<ModelInstance>();
	ground->setModel(groundModel);
	ground->transform->setLocalPosition(glm::vec3(-2.5, -5, 0) + positionOffset);

	shared_ptr<Rigidbody> groundRigidBody = entity.createChild<Rigidbody>(
		ground->transform,
		0,
		CollisionShapes::GetBoxShape(5.0f, 0.5f, 5.0f)
		);
	Physics::getInstance()->addRigidBody(groundRigidBody);

	auto whiteColor = sre::Color{ 0.0f, 0.0f, 0.0f };
	auto blackColor = sre::Color{ 1.0f, 1.0f, 1.0f };
	for (int i = 0; i < 10; i++) {
		auto color = i%2==0 ? whiteColor : blackColor;
		shared_ptr<Model> FLModel = createBoxModel2(2.5f, 0.5f, 0.5f, color);
		shared_ptr<ModelInstance> FL = entity.createChild<ModelInstance>();
		FL->setModel(FLModel);
		FL->transform->setLocalPosition(glm::vec3(2.5f, -5, -4.5f + (1.0f * i)) + positionOffset);

		shared_ptr<Rigidbody> FLRigidBody = entity.createChild<FinishLine>(
			FL->transform,
			0,
			CollisionShapes::GetBoxShape(2.5f, 0.5f, 0.5f)
			);

		Physics::getInstance()->addRigidBody(FLRigidBody);
	}


}
