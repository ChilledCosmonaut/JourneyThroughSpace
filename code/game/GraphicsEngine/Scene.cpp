#include "Scene.h"

namespace Graphics{

    glm::vec3 lightPos = glm::vec3(0.0f, -0.5f, 1.0f);

    unsigned int loadCubemap(vector<std::string> faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char *data = stbi_load((std::string("../../assets/textures/SpaceSkybox/").append(faces[i])).c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    void Graphics::Scene::Render() {
        DisplaySkybox();
        DisplayLights();
        DisplayModels();
    }

    void Scene::DisplaySkybox() {
        glDepthMask(GL_FALSE);
        skybox.shader.use();
        // ... set view and projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),(float) 16 / (float) 9, 0.1f, 300.0f);
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skybox.shader.setMatrix("projection", projection);
        skybox.shader.setMatrix("view", view);
        glBindVertexArray(skybox.VAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
    }

    void Scene::DisplayLights() {

    }

    void Scene::DisplayModels() {
        for (auto & sceneModel : sceneModels) {
            sceneModel.second.first->use();
            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1920.0f / 1080.0f, 0.1f, 1000.0f);
            glm::mat4 view = camera.GetViewMatrix();
            sceneModel.second.first->setMatrix("projection", projection);
            sceneModel.second.first->setMatrix("view", view);

            sceneModel.second.first->setMatrix("model", *sceneModel.second.second);

            sceneModel.second.first->setVector("viewPos",glm::vec4(camera.Position, 1.0f));

            sceneModel.second.first->setVector3("dirLight.direction", -lightPos);

            sceneModel.second.first->setVector3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
            sceneModel.second.first->setVector3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
            sceneModel.second.first->setVector3("dirLight.specular", glm::vec3(0.7f, 0.7f, 0.7f));

            sceneModel.first.Draw((gl3::shader &) sceneModel.second);
        }
    }

    Scene::Scene() {
        SetUpSkybox();
    }

    void Scene::SetUpSkybox() {
        float skyboxVertices[] = {
                // positions
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
        };

        glGenVertexArrays(1, &skybox.VAO);
        glGenBuffers(1, &skybox.VBO);
        glBindVertexArray(skybox.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, skybox.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);

        skybox.texture = loadCubemap(skybox.faces);
    }

    [[maybe_unused]] const glm::vec3 &Scene::getDirectionalLightPositionAtIndex(int index) const {
        return directionalLightPositions[index];
    }

    [[maybe_unused]] void Scene::setDirectionalLightPosition(glm::vec3 &directionalLightPosition) {
        Scene::directionalLightPositions.push_back(directionalLightPosition);
    }

    [[maybe_unused]] const std::pair<Model, std::pair<const gl3::shader *, glm::mat4 *>> &Scene::getSceneModelAtIndex(int index) const {
        return sceneModels[index];
    }

    [[maybe_unused]] void Scene::AddSceneModels(const Model& model, const gl3::shader* shader, glm::mat4* modelMatrix) {
        std::pair<const gl3::shader *, glm::mat4 *> temporary(shader, modelMatrix);
        std::pair<Model, std::pair<const gl3::shader *, glm::mat4 *>> sceneModel(model, temporary);
        Scene::sceneModels.push_back(sceneModel);
    }

    Camera *Scene::getCamera() {
        return &camera;
    }
}

