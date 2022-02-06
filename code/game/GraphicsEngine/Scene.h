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

        [[maybe_unused]] [[nodiscard]] const glm::vec3 &getDirectionalLightPositionAtIndex(int index) const;

        [[maybe_unused]] void setDirectionalLightPosition(glm::vec3 &directionalLightPosition);

        [[maybe_unused]] [[nodiscard]] const std::pair<Model, std::pair<const gl3::shader *, glm::mat4 *>> &getSceneModelAtIndex(int index) const;

        [[maybe_unused]] void AddSceneModels(const Model& model, const gl3::shader* shader, glm::mat4* modelMatrix);

    private:
        void DisplaySkybox();
        void DisplayLights();
        void DisplayModels();
        void SetUpSkybox();

        vector<std::pair<Model, std::pair<const gl3::shader *, glm::mat4 *>>> sceneModels;
        Camera camera;
    public:
        [[nodiscard]] Camera *getCamera();

    private:
        vector<glm::vec3> directionalLightPositions;
        Skybox skybox;
    };
}

