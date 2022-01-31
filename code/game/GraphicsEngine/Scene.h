#pragma once

#include <iostream>

#include "Model.h"
#include "camera.h"

namespace Graphics{

    struct Skybox{
        gl3::shader shader  = gl3::shader("shaders/SkyBoxVertexShader.glsl", "shaders/SkyBoxFragmentShader.glsl");
        unsigned int VAO, VBO;
        vector<std::string> faces;
        unsigned int texture;
        float vertices[];
    };

    class Scene {
    public:
        Scene();
        void Render();

    private:
        void DisplaySkybox();
        void DisplayLights();
        void DisplayModels();
        void SetUpSkybox();

        vector<std::pair<Model,gl3::shader>> sceneModels;
        Camera camera;
        Skybox skybox;
    };
}

