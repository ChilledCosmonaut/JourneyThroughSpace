#pragma once

#include <iostream>

#include "Model.h"
#include "camera.h"

namespace Graphics{

    class Scene {
    public:
        void Render();

    private:
        void DisplaySkybox();
        void DisplayLights();
        void DisplayModels();

        vector<std::pair<Model,gl3::shader>> sceneModels;
        Camera camera;
    };
}

