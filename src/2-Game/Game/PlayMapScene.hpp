#pragma once

#include <memory>
#include <string>
#include <ctime>

#include <Core/Scene.hpp>

namespace fg::game {

    class Character;
    class DebugCamera;

    class PlayMapScene : public core::Scene {
    public:

        PlayMapScene(const std::string& mapString, core::GameController& gameController)
            :   Scene(gameController),
                mapString(mapString)
        { }

    public:

        void onStart() override;

        void onRenderGui() override;

    private:

        void restart();

        const std::string mapString;

        std::weak_ptr<Character> character;

        std::weak_ptr<DebugCamera> debugCamera;

        bool finished = false;

		clock_t startTime = 0;

		float currentTime = 0;

    };

}