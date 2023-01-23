#pragma once

#include <memory>
#include <unordered_map>

namespace sre {
    class Mesh;
}

namespace fg::core {

    class Transform;
    class Model;


    class ModelPose {
    public:

        ModelPose(
            std::shared_ptr<const Model> model,
            const std::unordered_map<int, Transform>& meshTransforms
        );

        Transform& getMeshTransform(const std::shared_ptr<const sre::Mesh>& mesh);

        const Transform& getMeshTransform(const std::shared_ptr<const sre::Mesh>& mesh) const;

        const std::shared_ptr<const Model>& getModel() const;

    private:

        const std::shared_ptr<const Model> model;

        std::unordered_map<std::shared_ptr<const sre::Mesh>, Transform> meshTransforms;

    };

}