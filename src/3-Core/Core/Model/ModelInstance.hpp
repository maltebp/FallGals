#pragma once

#include <memory>

#include <Core/Entity.hpp>
#include <Core/Transform.hpp>

namespace sre {
    class Mesh;
    class Material;
}

namespace fg::core {

    class Model;
    class ModelPose;


    class ModelInstance : public Entity {
    public:
        ModelInstance();
        void setModel(std::shared_ptr<Model> model);

        const std::shared_ptr<Model>& getModel() const;

        void setMaterialOverride(
            std::shared_ptr<sre::Material> materialToOverride, 
            std::shared_ptr<sre::Material> overridingMaterial
        );

        void setPose(std::shared_ptr<const ModelPose> pose);

        std::shared_ptr<Transform> transform;

    protected:

        void onRender(Renderer& renderer) override;

    private:

        std::shared_ptr<Model> model;

        std::unordered_map<std::shared_ptr<sre::Material>, std::shared_ptr<sre::Material>> materialOverrides;

        std::shared_ptr<const ModelPose> pose;

    };
}