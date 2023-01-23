#pragma once

#include <memory>

namespace sre {
    class RenderPass;
    class Inspector;
}


namespace fg::core {

    class Scene;
    class GameController;
    
    // Exists to make it easier to extend rendering functionality, without
    // having to change the declaration of Entity class and all derived classes
    class Renderer {

        friend GameController;

    public:

        // This is only available (i.e. not nullptr) when rendering
        sre::RenderPass* getRenderPass();

    private:

        void renderScene(Scene& scene, sre::Inspector* sreInspector);                

        sre::RenderPass* renderPass;    

    };

}