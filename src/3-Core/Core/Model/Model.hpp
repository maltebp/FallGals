#pragma once

#include <memory>
#include <vector>

namespace sre {
    class Mesh;
    class Material;
}

namespace fg::core {


    struct MeshWithMaterial {

        std::shared_ptr<sre::Mesh> mesh;

        std::shared_ptr<sre::Material> material;

    }; 

    // A group of meshes with materials
    class Model {
    public:

        void addMesh(std::shared_ptr<sre::Mesh> mesh, std::shared_ptr<sre::Material> material);

        const std::vector<MeshWithMaterial>& getMeshes() const; 

    private:

        std::vector<MeshWithMaterial> meshes;

    };

}