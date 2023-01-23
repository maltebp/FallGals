#include <Core/EntityGroup.hpp>
#include <string>

namespace fg::game {

    static class MapLoader {
    public:
        static void LoadMap(fg::core::EntityGroup& entity, std::string map);
    };
}