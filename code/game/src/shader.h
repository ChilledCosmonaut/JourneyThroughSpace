
#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <string>

#include "../FileManager/FileManager.h"


namespace gl3{
    class shader {
    public:
        shader(const std::filesystem::path &vertexShaderAsset, const std::filesystem::path &fragmentShaderAsset);

        // explicit move constructor
        shader(shader &&other) noexcept {
            std::swap(this->shaderProgram, other.shaderProgram);
            std::swap(this->vertexShader, other.vertexShader);
            std::swap(this->fragmentShader, other.fragmentShader);
        }

        void use() const;

        void setVector(const std::string &uniform, glm::vec4 vector) const;
        void setVector3(const std::string &uniform, glm::vec3 vector) const;
        void setMatrix(const std::string &uniform, glm::mat4 matrix) const;
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;

        ~shader();

    private:
        unsigned int shaderProgram = 0;
        unsigned int vertexShader = 0;
        unsigned int fragmentShader = 0;
    };
}
