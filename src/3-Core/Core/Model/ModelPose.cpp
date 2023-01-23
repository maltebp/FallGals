#include "ModelPose.hpp"

#include <cassert>

#include <sre/Mesh.hpp>
#include <Core/Transform.hpp>
#include <Core/Model/Model.hpp>

using namespace std;
using namespace glm;
using namespace sre;
using namespace fg::core;


unordered_map<shared_ptr<const sre::Mesh>, Transform> createMeshTransformMap(
    const shared_ptr<const Model>& model,
    const unordered_map<int, Transform>& indexedMeshTransforms
) {
    assert(model != nullptr);

    const vector<MeshWithMaterial>& meshes = model->getMeshes();
    assert(
        meshes.size() == indexedMeshTransforms.size() &&
        "Number of pose transforms does not match model meshses"
    );

    unordered_map<shared_ptr<const Mesh>, Transform> meshTransforms;
    
    for( auto& [meshIndex, transform] : indexedMeshTransforms ) {
        const shared_ptr<const Mesh> mesh = meshes[meshIndex].mesh;
        meshTransforms[mesh] = transform;
    }

    return meshTransforms;
};


ModelPose::ModelPose(
    shared_ptr<const Model> model,
    const unordered_map<int, Transform>& meshTransforms
) 
    :   model(model),
        meshTransforms(createMeshTransformMap(model, meshTransforms))
{ }


Transform& ModelPose::getMeshTransform(const shared_ptr<const Mesh>& mesh) {
    auto it = meshTransforms.find(mesh);

    assert(it != meshTransforms.end() && "Invalid mesh");

    return it->second;
};


const Transform& ModelPose::getMeshTransform(const shared_ptr<const Mesh>& mesh) const {
    auto it = meshTransforms.find(mesh);

    assert(it != meshTransforms.end() && "Invalid mesh");

    return it->second;
};


const std::shared_ptr<const Model>& ModelPose::getModel() const {
    return model;
}