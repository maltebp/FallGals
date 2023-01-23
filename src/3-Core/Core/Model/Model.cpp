#include "Model.hpp"

#include <sre/RenderPass.hpp>

using namespace std;
using namespace sre;
using namespace fg::core;


void Model::addMesh(shared_ptr<Mesh> mesh, shared_ptr<Material> material) {

    MeshWithMaterial meshWithMaterial;
    meshWithMaterial.mesh = mesh;
    meshWithMaterial.material = material;

    meshes.push_back(meshWithMaterial);
}


const std::vector<MeshWithMaterial>& Model::getMeshes() const {
    return meshes;
}
