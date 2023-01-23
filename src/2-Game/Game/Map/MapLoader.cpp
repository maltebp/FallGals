#include "MapLoader.hpp"

#include <Game/Map/MapSegment.hpp>


void fg::game::MapLoader::LoadMap(fg::core::EntityGroup& rootEntity, std::string map)
{


    BlankSegment::addMapSegment(glm::vec3(0, 0, 0), rootEntity);


    for (int i = 0; i < map.size(); i++) {
        switch (map[i]) {
            case 'B':
            case 'b':
                DangerBridgeSegment::addMapSegment(glm::vec3(10 + (i*10), 0, 0), rootEntity);
                continue;
            case 'P':
            case 'p':
                BallPitSegment::addMapSegment(glm::vec3(10 + (i * 10), 0, 0), rootEntity);
                continue;
            case 'R':
            case 'r':
                RevolvingDoorSegment::addMapSegment(glm::vec3(10 + (i * 10), 0, 0), rootEntity);
                continue;
            case 'J':
            case 'j':
                RotatingPlatformsSegment::addMapSegment(glm::vec3(10 + (i * 10), 0, 0), rootEntity);
                continue;
            default:
                BlankSegment::addMapSegment(glm::vec3(10 + (i * 10), 0, 0), rootEntity);
        }
    }

    FinishLineSegment::addMapSegment(glm::vec3(10+(map.size() * 10), 0, 0), rootEntity);
}
