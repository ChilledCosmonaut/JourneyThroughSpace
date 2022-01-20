#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"


double deltaTime;

const float W_WIDTH = 800.0f;
const float W_HEIGHT = 600.0f;
const char* W_TITLE = "GameLab III";

float zRotation = 0.0f;
float rotStep = 90.0f;
float xTranslate = 0.0f, yTranslate = 0.0f;
float transStep = 1.0f;

const char* vShaderSource = "#version 460 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "uniform mat4 model;\n"
                            "uniform mat4 view;\n"
                            "uniform mat4 projection;\n"
                            "void main()\n"
                            "{\n"
                            "    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                            "}\n";

const char* fShaderSource = "#version 460 core\n"
                            "out vec4 fragColor;\n"
                            "void main()\n"
                            "{\n"
                            "   fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
                            "}\n";


void modelTransform(unsigned int shaderProgram);//gl3::shader* shaderProgram);
void viewTransform(unsigned int shaderProgram);//gl3::shader* shaderProgram);
void projectionTransform(unsigned int shaderProgram);//gl3::shader* shaderProgram);


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processUserInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // user input
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        zRotation -= rotStep * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        zRotation += rotStep * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        yTranslate += sin(glm::radians(zRotation)) * transStep * deltaTime;
        xTranslate += cos(glm::radians(zRotation)) * transStep * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        yTranslate -= sin(glm::radians(zRotation)) * transStep * deltaTime;
        xTranslate -= cos(glm::radians(zRotation)) * transStep * deltaTime;
    }
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, W_TITLE, nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Vertex Shader
    unsigned int vShader;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vShaderSource, nullptr);
    glCompileShader(vShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vShader, 512, nullptr, infoLog);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
    }

    // Fragment Shader
    unsigned int fShader;
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fShaderSource, nullptr);
    glCompileShader(fShader);

    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
    }

    // SHader Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout<<"ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << std::endl;
    }

    glUseProgram(shaderProgram);

    /*gl3::shader shaderProgram = gl3::shader("shaders/vertexShader.glsl","shaders/fragmentShader.glsl");
    shaderProgram.use();*/

    // Space Ship vertices
    float vertices[] = {
            0.5f, 0.025f, 0.0f,
            0.0f, 0.3f, 0.0f,
            -0.2f, 0.05f, 0.0f,

            0.5f, -0.025f, 0.0f,
            0.0f, -0.3f, 0.0f,
            -0.2f, -0.05f, 0.0f,
    };



    //Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Vertex Attribute Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glfwSetTime(1.0/60);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processUserInput(window);

        modelTransform(shaderProgram);
        viewTransform(shaderProgram);
        projectionTransform(shaderProgram);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // update deltaTime
        deltaTime = glfwGetTime();
        glfwSetTime(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void modelTransform(unsigned int shaderProgram){//gl3::shader* shaderProgram){

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(xTranslate, yTranslate, 0.0f));
    model = glm::rotate(model, glm::radians(zRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

    //shaderProgram->setMatrix("model", model);

    int modelLocation = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
}

void viewTransform(unsigned int shaderProgram){//gl3::shader* shaderProgram){

    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f,0.0f,25.0f),
                       glm::vec3(0.0f,0.0f,0.0f),
                       glm::vec3(0.0f,1.0f,0.0f));/*glm::vec3(0.0f,0.0f,0.0f),
                       glm::vec3(0.0f,0.0f,-1.0f),
                       glm::vec3(0.0f,1.0f,0.0f));*/

    //shaderProgram->setMatrix("view",view);

    int viewLocation = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
}

void projectionTransform(unsigned int shaderProgram){//gl3::shader* shaderProgram){

    glm::mat4 projection = glm::perspective(glm::radians(15.0f), W_WIDTH/W_HEIGHT, 0.1f, 100.0f);

    //shaderProgram->setMatrix("projection", projection);

    int projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}

