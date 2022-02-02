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
            sceneModel.second->use();
            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1920.0f / 1080.0f, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            sceneModel.second->setMatrix("projection", projection);
            sceneModel.second->setMatrix("view", view);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(-90.0f),glm::vec3(0,1.0f,1.0f));
            model = glm::translate(model, glm::vec3(5.0f, 0.0f, -55.0f)); // translate it down, so it's at the center of the scene
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
            sceneModel.second->setMatrix("model", model);

            sceneModel.second->setVector("viewPos",glm::vec4(camera.Position, 1.0f));

            sceneModel.second->setVector3("dirLight.direction", -lightPos);

            sceneModel.second->setVector3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
            sceneModel.second->setVector3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
            sceneModel.second->setVector3("dirLight.specular", glm::vec3(0.7f, 0.7f, 0.7f));

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

    [[maybe_unused]] const std::pair<Model, const gl3::shader *> &Scene::getSceneModelAtIndex(int index) const {
        return sceneModels[index];
    }

    [[maybe_unused]] void Scene::AddSceneModels(const Model& model, const gl3::shader* shader) {
        pair<Model, const gl3::shader *> sceneModel(model, shader);
        Scene::sceneModels.push_back(sceneModel);
    }

    Camera *Scene::getCamera() {
        return &camera;
    }
}

