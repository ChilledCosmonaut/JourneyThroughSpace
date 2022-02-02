
#pragma once

#include "shader.h"


namespace gl3{

    struct glStatusData {
        int success;
        const char* shaderName;
        char infoLog[GL_INFO_LOG_LENGTH];
    };

    unsigned int loadAndCompileShader(GLuint shaderType, const std::filesystem::path &shaderAssetPath){

        auto shaderAsset = files::FileManager::getAssetFileFrom(shaderAssetPath);
        const char* shaderSource = shaderAsset.c_str();
        unsigned int shader = glCreateShader(shaderType);

        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);

        {
            glStatusData compilationStatus{};

            if(shaderType == GL_VERTEX_SHADER) compilationStatus.shaderName = "VertexShader";
            if(shaderType == GL_FRAGMENT_SHADER) compilationStatus.shaderName = "FragmentShader";

            glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationStatus.success);
            if(compilationStatus.success == GL_FALSE){
                glGetShaderInfoLog(shader, GL_INFO_LOG_LENGTH, nullptr, compilationStatus.infoLog);
                throw std::runtime_error("ERROR::SHADER::" + std::string(compilationStatus.shaderName)
                                                                    + "::COMPILATION_FAILDE\n"
                                                                    + std::string(compilationStatus.infoLog));
            }
        }

        return shader;
    }

    shader::shader(const std::filesystem::path &vertexShaderAsset, const std::filesystem::path &fragmentShaderAsset) {

        // Load and compile shader
        vertexShader = loadAndCompileShader(GL_VERTEX_SHADER, vertexShaderAsset);
        fragmentShader = loadAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderAsset);

        // Create a shader program, attach the shaders and link program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Handle the errors and throw exceptions
        {
            glStatusData linkingStatus{};
            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkingStatus.success);
            if(linkingStatus.success == GL_FALSE){
                glGetShaderInfoLog(shaderProgram, GL_INFO_LOG_LENGTH, nullptr, linkingStatus.infoLog);
                throw std::runtime_error("ERROR::PROGRAM::LINKING_FAILED\n" + std::string(linkingStatus.infoLog));
            }
        }

        // Detach the shaders
        glDetachShader(shaderProgram, vertexShader);
        glDetachShader(shaderProgram, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void shader::use() const {
        glUseProgram(shaderProgram);
    }

    void shader::setVector(const std::string &uniform, glm::vec4 vector) const {
        unsigned int uniformLocation = glGetUniformLocation(shaderProgram, uniform.c_str());
        glUniform4f(uniformLocation, vector.x, vector.y, vector.z, vector.w);
    }

    void shader::setVector3(const std::string &uniform, glm::vec3 vector) const {
        unsigned int uniformLocation = glGetUniformLocation(shaderProgram, uniform.c_str());
        glUniform3f(uniformLocation, vector.x, vector.y, vector.z);
    }

    void shader::setMatrix(const std::string &uniform, glm::mat4 matrix) const {
        unsigned int uniformLocation = glGetUniformLocation(shaderProgram, uniform.c_str());
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    shader::~shader() {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(shaderProgram);
    }

    void shader::setBool(const std::string &name, bool value) const {
        unsigned int uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
        glUniform1i(uniformLocation, (int) value);
    }

    void shader::setInt(const std::string &name, int value) const {
        unsigned int uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
        glUniform1i(uniformLocation, (int) value);
    }

    void shader::setFloat(const std::string &name, float value) const {
        unsigned int uniformLocation = glGetUniformLocation(shaderProgram, name.c_str());
        glUniform1f(uniformLocation, value);
    }
}