#pragma once

#include <iostream>

#include "Model.h"
#include "camera.h"

namespace Graphics{

    struct Skybox{
        gl3::shader shader  = gl3::shader("shaders/SkyBoxVertexShader.glsl", "shaders/SkyBoxFragmentShader.glsl");
        unsigned int VAO, VBO;
        vector<std::string> faces{
                "AllSky_Space_AnotherPlanet_Cam_3_Right-X.png",
                "AllSky_Space_AnotherPlanet_Cam_2_Left+X.png",
                "AllSky_Space_AnotherPlanet_Cam_4_Up+Y.png",
                "AllSky_Space_AnotherPlanet_Cam_5_Down-Y.png",
                "AllSky_Space_AnotherPlanet_Cam_0_Front+Z.png",
                "AllSky_Space_AnotherPlanet_Cam_1_Back-Z.png"
        };
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

