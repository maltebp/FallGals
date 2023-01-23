#include "CanonObstacle.hpp"
#include <Core/GameTime.hpp>
#include <sre/Mesh.hpp>
#include <sre/Shader.hpp>

#include <Core/Model/Model.hpp>
#include <Utility/Math.hpp>
#include <Utility/MeshShapes.hpp>
#include <Utility/CollisionShapes.hpp>
#include <Core/Rigidbody.hpp>
#include <Core/Physics.hpp>
#include <Game/Map/CannonBall.hpp>

using namespace std;
using namespace glm;
using namespace fg::core;
using namespace fg::game;
using namespace fg::utility;

fg::game::CanonObstacle::CanonObstacle(glm::vec3 fAngle)
{
	firingSpeed = 1;
	firingAngle = fAngle;
	timeSinceLastFire = 0;
}

void fg::game::CanonObstacle::onUpdate(const fg::core::GameTime& time)
{
	timeSinceLastFire += time.frameTime;

	if (timeSinceLastFire > firingSpeed) {
		timeSinceLastFire -= firingSpeed;

		vec3 ballImpulse = (firingAngle + glm::vec3(Math::randomFloat(-1, 1), Math::randomFloat(-1, 1), Math::randomFloat(-1, 1))) * 5.0f;

		shared_ptr<CannonBall> ball = createChild<CannonBall>(
			transform->getLocalPosition(),
			ballImpulse
		);
		Physics::getInstance()->addRigidBody(ball);
	}
}
