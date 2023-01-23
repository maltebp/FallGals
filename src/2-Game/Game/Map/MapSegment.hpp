#include <Core/EntityGroup.hpp>
#include <Core/Model/ModelInstance.hpp>




namespace fg::game {
	static class MapSegment {
	public:
		static void addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup &entity);
	};

	static class BlankSegment : MapSegment {
	public:
		static void addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup& entity);
	};

	static class RotatingPlatformsSegment : MapSegment {
	public:
		static void addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup& entity);
	};

	static class BallPitSegment : MapSegment {
	public:
		static void addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup& entity);
	};

	static class RevolvingDoorSegment : MapSegment {
	public:
		static void addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup& entity);
	};

	static class DangerBridgeSegment : MapSegment {
	public:
		static void addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup& entity);
	};

	static class FinishLineSegment : MapSegment {
	public:
		static void addMapSegment(glm::vec3 positionOffset, fg::core::EntityGroup& entity);
	};
}