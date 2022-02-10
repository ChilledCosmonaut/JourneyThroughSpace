#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "../FileManager/stb_image.h"
#include "../GraphicsEngine/camera.h"
#include "../GraphicsEngine/Model.h"
#include "../GraphicsEngine/Scene.h"
#include "../InputSystem/InputManager.h"
#include <iostream>

double deltaTime;

const float W_WIDTH = 1920.0f;
const float W_HEIGHT = 1080.0f;
const char *W_TITLE = "GameLab III";

// camera
Camera *camera;
float lastX = W_WIDTH / 2.0f;
float lastY = W_HEIGHT / 2.0f;
bool firstMouse = true;

float zRotation = 0.0f;
float rotStep = 90.0f;
float xTranslate = 0.0f, yTranslate = 0.0f;
float transStep = 20.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lightPos = glm::vec3(0.0f, -0.5f, 1.0f);
glm::vec3 shipPos1 = glm::vec3(5.0f, -95.0f, -55.0f);
glm::vec3 shipPos2 = glm::vec3(20.0f, -65.0f, -50.0f);
glm::vec3 shipPos3 = glm::vec3(40.0f, -90.0f, -60.0f);

float yaw = -90.0f, pitch = 0.0f, fov = 45.0f;


void modelTransform(gl3::shader *shaderProgram);//unsigned int shaderProgram);
void viewTransform(gl3::shader *shaderProgram);//unsigned int shaderProgram);
void projectionTransform(gl3::shader *shaderProgram);//unsigned int shaderProgram);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processUserInput(GLFWwindow *window, int key, int scancode, int action, int mods) {

    const float cameraSpeed = 0.5f * deltaTime; // adjust accordingly

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // user input
    /*if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->ProcessKeyboard(RIGHT, deltaTime);
        //std::cout<<"Pressed D" + to_string(deltaTime) <<endl;
        //cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        //zRotation -= rotStep * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->ProcessKeyboard(LEFT, deltaTime);
        //cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        //zRotation += rotStep * deltaTime;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->ProcessKeyboard(FORWARD, deltaTime);
        *//*yTranslate += sin(glm::radians(zRotation)) * transStep * deltaTime;
        xTranslate += cos(glm::radians(zRotation)) * transStep * deltaTime;*//*
        //cameraPos += cameraSpeed * cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->ProcessKeyboard(BACKWARD, deltaTime);
        //cameraPos -= cameraSpeed * cameraFront;
        *//*yTranslate -= sin(glm::radians(zRotation)) * transStep * deltaTime;
        xTranslate -= cos(glm::radians(zRotation)) * transStep * deltaTime;*//*
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        shipPos1.z += transStep * deltaTime;
        shipPos2.z += transStep * deltaTime;
        shipPos3.z += transStep * deltaTime;
    }*/
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(W_WIDTH, W_HEIGHT, W_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);

    input::InputManager inputCallback = input::InputManager();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glfwSetCursorPosCallback(window, mouse_callback);
    inputCallback.AddMouseMoveCallback(mouse_callback);

    //glfwSetScrollCallback(window, scroll_callback);
    inputCallback.AddScrollCallback(scroll_callback);

    inputCallback.AddKeyboardCallback(processUserInput);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    gl3::shader litShader = gl3::shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    //Model model = Model("../../assets/TransportShip.fbx");
    Model model1 = Model("../../assets/SpaceShip4.obj");
    Model model2 = Model("../../assets/SpaceShip2.obj");
    Model model3 = Model("../../assets/SpaceShip3.obj");


    float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int VBO, lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    Graphics::Scene scene = Graphics::Scene();

    camera = scene.getCamera();

    gl3::shader shader = gl3::shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(180.0f),glm::vec3(0,1.0f,0));
    model = glm::rotate(model, glm::radians(-90.0f),glm::vec3(1.0f,0,0));
    model = glm::translate(model, shipPos1);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    litShader.setMatrix("model", model);
    model1.Draw(litShader);

    scene.AddSceneModels(model1, &shader, &model);

    inputCallback.StartListening(window);


    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.Render();
        // ... draw rest of the scene

        // don't forget to enable shader before setting uniforms
        /*litShader.use();

        litShader.setVector("viewPos",glm::vec4(camera.Position, 1.0f));

        litShader.setVector3("dirLight.direction", -lightPos);

        litShader.setVector3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        litShader.setVector3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        litShader.setVector3("dirLight.specular", glm::vec3(0.7f, 0.7f, 0.7f));



        // render the loaded model
        /*glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-90.0f),glm::vec3(0,1.0f,1.0f));
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        litShader.setMatrix("model", model);
        model.Draw(litShader);*/

        /*

        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(180.0f),glm::vec3(0,1.0f,0));
        model = glm::rotate(model, glm::radians(-90.0f),glm::vec3(1.0f,0,0));
        model = glm::translate(model, shipPos2);
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        litShader.setMatrix("model", model);
        model2.Draw(litShader);

        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(180.0f),glm::vec3(0,1.0f,0));
        model = glm::rotate(model, glm::radians(-90.0f),glm::vec3(1.0f,0,0));
        model = glm::translate(model, shipPos3);
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        litShader.setMatrix("model", model);
        model3.Draw(litShader);

        // also draw the lamp object
        /*lightShader.use();
        lightShader.setMatrix("projection", projection);
        lightShader.setMatrix("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightShader.setMatrix("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lightShader.setMatrix("projection", projection);
        lightShader.setMatrix("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,0.0f,0.0f));
        model = glm::scale(model, glm::vec3(0.4f)); // a smaller cube
        lightShader.setMatrix("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);*/

        // update deltaTime
        deltaTime = glfwGetTime();
        glfwSetTime(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void modelTransform(gl3::shader *shaderProgram) {//unsigned int shaderProgram){

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    //model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

    shaderProgram->setMatrix("model", model);

    /*int modelLocation = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));*/
}

void viewTransform(gl3::shader *shaderProgram) {//unsigned int shaderProgram){

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    /*view = glm::lookAt(glm::vec3(0.0f,0.0f,25.0f),
                       glm::vec3(0.0f,0.0f,0.0f),
                       glm::vec3(0.0f,1.0f,0.0f));//glm::vec3(0.0f,0.0f,0.0f),
                       glm::vec3(0.0f,0.0f,-1.0f),
                       glm::vec3(0.0f,1.0f,0.0f));*/

    shaderProgram->setMatrix("view", view);

    /*int viewLocation = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));*/
}

void projectionTransform(gl3::shader *shaderProgram) {//unsigned int shaderProgram){

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), W_WIDTH / W_HEIGHT, 0.1f, 100.0f);

    shaderProgram->setMatrix("projection", projection);

    /*int projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));*/
}