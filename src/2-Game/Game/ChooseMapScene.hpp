#pragma once

#include <string>

#include <Core/Scene.hpp>

namespace fg::game {

    class ChooseMapScene : public core::Scene {
    public:

        ChooseMapScene(core::GameController& gameController) 
            :   Scene(gameController)  
        { }

    public:

        void onRenderGui() override;

    private:

        void loadMap(const std::string& map);
        
    };

}