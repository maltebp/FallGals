#include <sre/RenderPass.hpp>

namespace fg::utility {
	class MeshShapes {
	public:
		static std::shared_ptr<sre::Mesh> GetBoxShape(float x_size, float y_size, float z_size);
		static std::shared_ptr<sre::Mesh> GetBoxShape(float size);
		static std::shared_ptr<sre::Mesh> GetCyllinderShape(float x_size, float y_size, float z_size);
		static std::shared_ptr<sre::Mesh> GetCyllinderShape(float size);
		static std::shared_ptr<sre::Mesh> GetCapsuleShape(float radius, float height);
		static std::shared_ptr<sre::Mesh> GetSphereShape(float size);
	};
}