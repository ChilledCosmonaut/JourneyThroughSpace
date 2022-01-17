
#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>


namespace gl3{
    class shader {
    public:
        shader(const std::filesystem::path &vertexShaderAsset, const std::filesystem::path &fragmentShaderAsset);

        // delete copy constructor
        shader(const shader &shader) = delete;

        // explicit move constructor
        shader(shader &&other) noexcept {
            std::swap(this->shaderProgram, other.shaderProgram);
            std::swap(this->vertexShader, other.vertexShader);
            std::swap(this->fragmentShader, other.fragmentShader);
        }

        void use() const;

        void setVector(const std::string &uniform, glm::vec4 matrix) const;
        void setMatrix(const std::string &uniform, glm::mat4 vector) const;

        ~shader();

    private:
        unsigned int shaderProgram = 0;
        unsigned int vertexShader = 0;
        unsigned int fragmentShader = 0;
    };
}
