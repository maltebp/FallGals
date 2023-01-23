#pragma once

#include <iostream>
#include <functional>

#include <External/catch2.hpp>

#include <Core/Entity.hpp>
#include <Core/GameTime.hpp>

using namespace std;
using namespace fg::core;


class TestEntity : public Entity {
public:

    TestEntity(int levels, int numChildren) {
        if( levels <= 1) return;

        for( int i = 0; i < numChildren; i++) {
            createChild<TestEntity>(levels - 1, numChildren);
        }        
    }

    ~TestEntity(){
        if (destructorCallback != nullptr) {
            destructorCallback(*this);
        }
    }


    void onUpdate(const GameTime& time) {
        timePassed =+ time.frameTime;
    }


    void destroy() {
        Entity::destroy();
    }

    
    void destroy(std::function<void(TestEntity&)> destructorCallback) {
        this->destructorCallback = destructorCallback;
        Entity::destroy();
    }


    void forSelfAndChildrenRecursively(function<void(TestEntity&)> action) {
        action(*this);
        for( auto& rawChild : getChildrenCopy() ) {
            dynamic_pointer_cast<TestEntity>(rawChild)->forSelfAndChildrenRecursively(action);
        }
    }

    std::function<void(TestEntity&)> destructorCallback;

    float timePassed = 0.0f;

};


int getNumEntities(TestEntity& root) {
    int entityCount = 0;
    root.forSelfAndChildrenRecursively([&entityCount](TestEntity& entity){
        entityCount++;
    });
    return entityCount;
}


TEST_CASE("General creation, update and deletion") {

    int levels = 3;
    int numChildren = 3;

    shared_ptr<TestEntity> rootEntity = std::make_shared<TestEntity>(levels, numChildren);
    
    int expectedEntities = (int)(pow(numChildren, levels) - 1) / (numChildren - 1);

    REQUIRE( getNumEntities(*rootEntity) == expectedEntities);

    GameTime gameTime;
    gameTime.frameTime = 1.0f;
    rootEntity->update(gameTime);
    rootEntity->forSelfAndChildrenRecursively([&gameTime](TestEntity& entity) {
        REQUIRE(entity.timePassed == gameTime.frameTime);
    });

    bool entityDestroyed = false;
    auto onEntityDestroyed = [&entityDestroyed](TestEntity& entity) {
        REQUIRE( entityDestroyed != true);
        entityDestroyed = true;
    };

    int currentEntity = 0;
    rootEntity->forSelfAndChildrenRecursively([&currentEntity, expectedEntities, onEntityDestroyed](TestEntity& entity) {
        currentEntity++;

        // Delete last entity
        if( currentEntity == expectedEntities ) {
            entity.destroy(onEntityDestroyed);
        }
    });

    REQUIRE( getNumEntities(*rootEntity) == expectedEntities);

    rootEntity->clean();

    REQUIRE( getNumEntities(*rootEntity) == (expectedEntities - 1));
    REQUIRE( entityDestroyed == true );  

}


class BaseEntity : public Entity {
public: 

    using Entity::createChild;

    // template<class TEntity>
    // const std::shared_ptr<TEntity>& createChild() {
    //     return Entity::createChild<TEntity>();
    // }

};

class EntityA : public BaseEntity { };

class EntityB : public BaseEntity { };

class EntityC : public BaseEntity { };


TEST_CASE("Iterate children") {

    EntityA rootEntity;

    shared_ptr<EntityA> entityA = rootEntity.createChild<EntityA>();
    entityA->createChild<EntityA>();
    entityA->createChild<EntityB>();
    entityA->createChild<EntityB>();
    entityA->createChild<EntityC>();
    entityA->createChild<EntityC>();
    entityA->createChild<EntityC>();

    shared_ptr<EntityB> entityB = rootEntity.createChild<EntityB>();
    entityB->createChild<EntityA>();
    entityB->createChild<EntityB>();
    entityB->createChild<EntityB>();
    entityB->createChild<EntityC>();
    entityB->createChild<EntityC>();
    entityB->createChild<EntityC>();

    shared_ptr<EntityC> entityC = rootEntity.createChild<EntityC>();
    entityC->createChild<EntityA>();
    entityC->createChild<EntityB>();
    entityC->createChild<EntityB>();
    entityC->createChild<EntityC>();
    entityC->createChild<EntityC>();
    entityC->createChild<EntityC>();

    int numEntityA = 0;
    rootEntity.forAllChildrenOfType<EntityA>([&](const shared_ptr<EntityA>& entity) { 
        numEntityA++;
    });
    REQUIRE(numEntityA == 4);

    int numEntityB = 0;
    rootEntity.forAllChildrenOfType<EntityB>([&](const shared_ptr<EntityB>& entity) { 
        numEntityB++;
    });
    REQUIRE(numEntityB == 7);

    int numEntityC = 0;
    rootEntity.forAllChildrenOfType<EntityC>([&](const shared_ptr<EntityC>& entity) { 
        numEntityC++;
    });
    REQUIRE(numEntityC == 10);

}


