#pragma once

#include <memory>
#include <vector>

#include <Core/Entity.hpp>

namespace fg::core {
	class Camera;
}

namespace fg::game {

	class Character;


	class CharacterController : public core::Entity {
	public:

		CharacterController(Character* character, core::Camera* camera);

	protected:

		bool onKeyEvent(SDL_KeyboardEvent& keyboardEvent) override;

		void onUpdate(const core::GameTime& time) override;

		bool onMouseMotionEvent(SDL_MouseMotionEvent& mouseMotionEvent) override;

	private:

		Character* character;

		core::Camera* camera;
		
		bool forward = false;
		
		bool left = false;
		
		bool right = false;
		
		bool back = false;
		
		float cameraXRotation = 0.f;
		
		float playerYRotation = 0.f;
		
		float rotationSpeed = 0.3f;
		
		float walkingSpeed = 1000.f;
		
		float maxXRotation = 15.f;
		
	};
	
}