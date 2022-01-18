#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "shader.h"


double deltaTime;

float zRotation = 0.0f;
float rotSpeed  = 120.0f;

const float W_WIDTH = 800.0f;
const float W_HEIGHT = 600.0f;
const char* W_TITLE = "GameLab III";


void modelTransform(gl3::shader* shaderProgram);
void viewTransform(gl3::shader* shaderProgram);
void projectionTransform(gl3::shader* shaderProgram);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processUserInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // user input
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        std::cout << "zRotation: " << zRotation << std::endl;
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        std::cout << "zRotation: " << zRotation << std::endl;
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        // add your own input here
    }
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "GameLabIII", nullptr, nullptr);
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

    gl3::shader shaderProgram = gl3::shader("shaders/vertexShader.glsl","shaders/fragmentShader.glsl");
    shaderProgram.use();

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glfwSetTime(1.0/60);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processUserInput(window);

        modelTransform(&shaderProgram);
        viewTransform(&shaderProgram);
        projectionTransform(&shaderProgram);

        // update deltaTime
        deltaTime = glfwGetTime();
        glfwSetTime(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void modelTransform(gl3::shader* shaderProgram){

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, 0.0f));
    model = glm::rotate(model, glm::radians(zRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

    shaderProgram->setMatrix("model", model);
}

void viewTransform(gl3::shader* shaderProgram){

    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),
                       glm::vec3(0.0f,0.0f,-1.0f),
                       glm::vec3(0.0f,1.0f,0.0f));

    shaderProgram->setMatrix("view",view);
}

void projectionTransform(gl3::shader* shaderProgram){

    glm::mat4 projection = glm::perspective(60.0f, W_WIDTH/W_HEIGHT, 0.1f, 100.0f);

    shaderProgram->setMatrix("projection", projection);
}

