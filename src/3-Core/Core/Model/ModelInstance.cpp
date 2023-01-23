#include "ModelInstance.hpp"

#include <cassert>

#include <glm/glm.hpp>
#include <sre/RenderPass.hpp>

#include <Core/Renderer.hpp>
#include <Core/Model/Model.hpp>
#include <Core/Model/ModelPose.hpp>

using namespace std;
using namespace glm;
using namespace sre;
using namespace fg::core;


fg::core::ModelInstance::ModelInstance()
{
    transform = make_shared<Transform>();
}

void ModelInstance::setModel(shared_ptr<Model> model) {
    this->model = model;
}


const shared_ptr<Model>& ModelInstance::getModel() const {
    return model;
}


void ModelInstance::setMaterialOverride(
    shared_ptr<Material> materialToOverride, 
    shared_ptr<Material> overridingMaterial
) {
    materialOverrides[materialToOverride] = overridingMaterial;
}


void ModelInstance::setPose(std::shared_ptr<const ModelPose> pose) {
    assert(pose == nullptr || pose->getModel() == model);
    this->pose = pose;
}


void ModelInstance::onRender(Renderer& renderer) {
    
    if( model == nullptr ) return;

    for( MeshWithMaterial meshWithMaterial : model->getMeshes() ) {

        shared_ptr<Mesh> mesh = meshWithMaterial.mesh;
        shared_ptr<Material> material = meshWithMaterial.material;

        auto it = materialOverrides.find(material);
        shared_ptr<Material> actualMaterial = 
            it != materialOverrides.end() ?
            it->second :
            material;

        mat4 actualTransform;

        if( pose != nullptr ) {
            const Transform& meshPoseTransform = pose->getMeshTransform(mesh);
            actualTransform = transform->getWorldMatrix() * meshPoseTransform.getModelMatrix();
        }
        else
        {
            actualTransform = transform->getWorldMatrix();
        }

        renderer.getRenderPass()->draw(
            mesh,
            actualTransform,
            actualMaterial
        );
    }
}