#pragma once

#include "Entity.hpp"


namespace fg::core {

    class EntityGroup : public Entity {
    public:

        EntityGroup() { }

        using Entity::createChild;

    };

}