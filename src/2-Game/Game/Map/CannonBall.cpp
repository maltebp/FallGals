#include "CannonBall.hpp"

#include <sre/Mesh.hpp>
#include <sre/Material.hpp>

#include <Utility/CollisionShapes.hpp>
#include <Core/Transform.hpp>
#include <Core/GameTime.hpp>
#include <Core/Physics.hpp>
#include <Core/Model/Model.hpp>
#include <Core/Model/ModelInstance.hpp>

using namespace std;
using namespace glm;
using namespace fg::utility;
using namespace fg::core;
using namespace fg::game;


CannonBall::CannonBall(vec3 position, vec3 initialImpulse) 
    :   Rigidbody(weak_ptr<Transform>(), 1.0f, CollisionShapes::GetSphereShape(0.5f)),
        transform(make_shared<Transform>())
{
    transform->setLocalPosition(position);
    _Transform = transform;

    shared_ptr<sre::Mesh> mesh = sre::Mesh::create()
        .withSphere(16, 32, 0.5f)
        .build();

    shared_ptr<sre::Material> material = sre::Shader::getStandardBlinnPhong()->createMaterial();
    material->setColor(sre::Color{
        (rand()) / static_cast<float>(RAND_MAX),
        (rand()) / static_cast<float>(RAND_MAX),
        (rand()) / static_cast<float>(RAND_MAX)}
    );
    material->setSpecularity(sre::Color{1, 1, 1, 50});

    shared_ptr<Model> model = make_shared<Model>();
    model->addMesh(mesh, material);

    shared_ptr<ModelInstance> modelInstance = createChild<ModelInstance>();
    modelInstance->setModel(model);
    modelInstance->transform->setParent(transform);

    applyImpulse(initialImpulse);
}

void CannonBall::onUpdate(const GameTime& gameTime) {
    if (transform->getWorldPosition().y < -10) {
        this->destroy();
    }
}