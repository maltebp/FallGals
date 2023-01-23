#include "CollisionShapes.hpp"

#include "btBulletDynamicsCommon.h"

using namespace std;
using namespace fg::utility;


shared_ptr<btCollisionShape> CollisionShapes::GetBoxShape(float x_size, float y_size, float z_size) {
	return make_shared<btBoxShape>(btVector3(x_size, y_size, z_size));
}


shared_ptr<btCollisionShape> CollisionShapes::GetBoxShape(float size) {
	return GetBoxShape(size, size, size);
}


shared_ptr<btCollisionShape> CollisionShapes::GetCyllinderShape(float x_size, float y_size, float z_size) {
	return make_shared<btCylinderShape>(btVector3(x_size, y_size, z_size));
}


shared_ptr<btCollisionShape> CollisionShapes::GetCyllinderShape(float size) {
	return GetCyllinderShape(size, size, size);
}


shared_ptr<btCollisionShape> CollisionShapes::GetCapsuleShape(float radius, float height) {
	return make_shared<btCapsuleShape>(radius, height);
}


shared_ptr<btCollisionShape> CollisionShapes::GetSphereShape(float size) {
	return make_shared<btSphereShape>(size);
}