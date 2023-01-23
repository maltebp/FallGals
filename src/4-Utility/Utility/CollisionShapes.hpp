#pragma once

#include <memory>

class btCollisionShape;

namespace fg::utility {
	
	class CollisionShapes {
	public:

		static std::shared_ptr<btCollisionShape> GetBoxShape(float x_size, float y_size, float z_size);
		
		static std::shared_ptr<btCollisionShape> GetBoxShape(float size);
		
		static std::shared_ptr<btCollisionShape> GetCyllinderShape(float x_size, float y_size, float z_size);
		
		static std::shared_ptr<btCollisionShape> GetCyllinderShape(float size);
		
		static std::shared_ptr<btCollisionShape> GetCapsuleShape(float radius, float height);
		
		static std::shared_ptr<btCollisionShape> GetSphereShape(float size);
	};
}