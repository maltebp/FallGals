#include "PlayMapScene.hpp"

#include <iostream>

#include <imgui.h>
#include <sre/RenderPass.hpp>

#include <Core/GameController.hpp>
#include <Core/WindowContext.hpp>
#include <Core/Renderer.hpp>
#include <Core/Model/Model.hpp>
#include <Core/Model/ModelInstance.hpp>
#include <Core/Rigidbody.hpp>
#include <Core/Camera.hpp>
#include <Game/DebugCamera.hpp>
#include <Utility/CollisionShapes.hpp>
#include <Game/Character/CharacterController.hpp>
#include <Game/Character/Character.hpp>
#include <Game/Character/CharacterAssets.hpp>
#include <Game/Map/MapLoader.hpp>
#include <Game/ChooseMapScene.hpp>

using namespace std;
using namespace glm;
using namespace fg::core;
using namespace fg::game;
using namespace fg::utility;


shared_ptr<Model> createCubeModel(float size, sre::Color color) {

    shared_ptr<sre::Mesh> mesh = sre::Mesh::create()
        .withCube(size)
        .build();

    shared_ptr<sre::Material> material = sre::Shader::getUnlit()->createMaterial();
    material->setColor(color);

    shared_ptr<Model> model = make_shared<Model>();
    model->addMesh(mesh, material);

    return model;
}


void PlayMapScene::onStart() {

    shared_ptr<game::CharacterAssets> characterAssets = make_shared<CharacterAssets>();
    shared_ptr<game::Character> character = rootEntity.createChild<Character>(
        characterAssets, [this]() { this->finished = true; } 
    );
    Physics::getInstance()->addRigidBody(character);

    shared_ptr<Model> groundModel = createCubeModel(50.f, sre::Color{ 0.7f, 0.5f, 1.0f });

    MapLoader::LoadMap(rootEntity, mapString);

    shared_ptr<DebugCamera> debugCamera = rootEntity.createChild<DebugCamera>(
        gameController.getWindowContext()
    );

    debugCamera->getTransform()->setLocalPosition(vec3{ 0, 0, 2.0f });
    this->debugCamera = debugCamera;
    this->activeCamera = character->camera;

    restart();
}


void PlayMapScene::onRenderGui() {

    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiSetCond_Always);
	ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    if( !finished ) {
        ImGui::Text("Reach the Finish Line");
		currentTime = (float)((std::clock() - startTime) / (double)CLOCKS_PER_SEC);
		ImGui::Text("Current time: %.2f seconds", currentTime);
    }
    else {
		ImGui::Text("You finished in %.2f seconds!", currentTime);
        
        if( ImGui::Button("Restart") ) {
            restart();
        }
    }

    if( ImGui::Button("Map selection") ) {
        shared_ptr<ChooseMapScene> chooseMapScene = make_shared<ChooseMapScene>(gameController);
        gameController.setScene(chooseMapScene);
    }

	ImGui::End();
}


void PlayMapScene::restart() {
    std::cout << "Restarting" << std::endl;

    finished = false;
    startTime = std::clock();
    
    if( auto characterPtr = character.lock() ) {
        characterPtr->setLinearVelocity(vec3{0});

        characterPtr->transform->setLocalPositionByWorldPosition(vec3{0});
        characterPtr->transform->setLocalRotation(quat{vec3{0, radians(180.0f), 0}});
    }
}