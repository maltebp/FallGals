#include "Renderer.hpp"

#include <cassert>

#include <sre/RenderPass.hpp>
#include <sre/Inspector.hpp>

#include <Core/Scene.hpp>
#include <Core/Camera.hpp>

using namespace std;
using namespace glm;
using namespace fg::core;


sre::RenderPass* Renderer::getRenderPass() {
    assert(renderPass != nullptr && "RenderPass is nullptr");
    return renderPass;
}


void Renderer::renderScene(Scene& scene, sre::Inspector* sreInspector) {

    shared_ptr<Camera> camera = scene.activeCamera.lock();

    sre::RenderPass::RenderPassBuilder& worldRenderPassBuilder = sre::RenderPass::create();
    
    worldRenderPassBuilder
        .withName("World Render Pass")
        .withGUI(false);
 
    if( camera != nullptr ) {

        sre::Camera& sreCamera = *camera->updateAndGetSreCamera().get();
            
        worldRenderPassBuilder.withCamera(sreCamera);
    
        sre::WorldLights sreWorldLights;
        sreWorldLights.addLight(
            sre::Light::create()
            .withDirectionalLight(glm::normalize(glm::vec3(1,1,1)))
            .build()
        );

        worldRenderPassBuilder.withWorldLights(&sreWorldLights);

        sre::RenderPass renderPass = worldRenderPassBuilder.build();

        this->renderPass = &renderPass;

        scene.render(*this);

        this->renderPass = nullptr;

    }
    else {
        // We build and render without scene to clear the screen
        sre::RenderPass renderPass = worldRenderPassBuilder.build();
    }

    sre::RenderPass renderPass = sre::RenderPass::create()
        .withClearColor(false)
        .withName("GUI Render Pass")
        .withGUI(true)
        .build();

    scene.renderGui();

    if( sreInspector != nullptr ) {
        sreInspector->gui();
    }
}