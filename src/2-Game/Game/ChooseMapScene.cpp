#include "ChooseMapScene.hpp"

#include <imgui.h>

#include <Core/GameController.hpp>
#include <Core/Renderer.hpp>
#include <Game/PlayMapScene.hpp>

using namespace std;
using namespace fg::core;
using namespace fg::game;


void ChooseMapScene::onRenderGui() {

    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiSetCond_Always);
    
    ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    ImGui::Text("Welcome to Fall Galls! Please select a map.");
    
    bool loadDefaultMap = ImGui::Button("Default MAP");
    bool loadSuperBridge = ImGui::Button("Super Danger Bridge");
    bool loadBallPit = ImGui::Button("Ballpit of Death");

    ImGui::End();

    if (loadDefaultMap) {
        loadMap("BPJR");
    }
    else if (loadSuperBridge) {
        loadMap("BBPBB");
    }
    else if (loadBallPit) {
        loadMap("PPJPP");
    }
}


void ChooseMapScene::loadMap(const string& map) {
    shared_ptr<PlayMapScene> playMapScene = make_shared<PlayMapScene>(map, gameController);
    gameController.setScene(playMapScene);
}
