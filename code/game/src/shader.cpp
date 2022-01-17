
#include <stdexcept>
#include "shader.h"


namespace gl3{

    struct glStatusData {
        int success;
        const char* shaderName;
        char infoLog[GL_INFO_LOG_LENGTH];
    };

    std::string readText(const std::filesystem::path &fileName) {
        std::ifstream sourceFile(fileName);
        std::stringstream buffer;
        buffer << sourceFile.rdbuf();
        return buffer.str();
    }

    unsigned int loadAndCompileShader(GLuint shaderType, const std::filesystem::path &shaderAssetPath){

        auto shaderAsset = readText(shaderAssetPath);
        const char* shaderSource = shaderAsset.c_str();
        unsigned int shader = glCreateShader(GL_VERTEX_SHADER);

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

        return  shader;
    }

    shader::shader(const std::filesystem::path &vertexShaderAsset, const std::filesystem::path &fragmentShaderAsset) {

        // Load and compile shader
        vertexShader = loadAndCompileShader(GL_VERTEX_SHADER, vertexShaderAsset);
        fragmentShader = loadAndCompileShader(GL_VERTEX_SHADER, fragmentShaderAsset);

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
    }

    void shader::use() const {
        glUseProgram(shaderProgram);
    }

    void shader::setVector(const std::string &uniform, glm::vec4 matrix) const {
        unsigned int uniformLocation = glGetUniformLocation(shaderProgram, uniform.c_str());
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void shader::setMatrix(const std::string &uniform, glm::mat4 vector) const {
        unsigned int uniformLocation = glGetUniformLocation(shaderProgram, uniform.c_str());
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(vector));
    }

    shader::~shader() {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
}