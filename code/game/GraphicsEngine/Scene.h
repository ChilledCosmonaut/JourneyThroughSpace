#pragma once

#include "Model.h"

namespace Graphics{

    class Scene {
    public:
        void Render();

    private:
        void DisplaySkybox();
        void DisplayLights();
        void DisplayModels();

        vector<std::pair<Model,gl3::shader>> sceneModels;
    };
}

