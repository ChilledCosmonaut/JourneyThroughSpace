#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "../FileManager/stb_image.h"
#include "camera.h"


double deltaTime;

const float W_WIDTH = 800.0f;
const float W_HEIGHT = 600.0f;
const char* W_TITLE = "GameLab III";

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = W_WIDTH / 2.0f;
float lastY = W_HEIGHT / 2.0f;
bool firstMouse = true;

float zRotation = 0.0f;
float rotStep = 90.0f;
float xTranslate = 0.0f, yTranslate = 0.0f;
float transStep = 1.0f;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 lightPos    = glm::vec3(1.2f, 1.0f,  2.0f);

float yaw = -90.0f, pitch = 0.0f, fov = 45.0f;


void modelTransform(gl3::shader* shaderProgram);//unsigned int shaderProgram);
void viewTransform(gl3::shader* shaderProgram);//unsigned int shaderProgram);
void projectionTransform(gl3::shader* shaderProgram);//unsigned int shaderProgram);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processUserInput(GLFWwindow *window){

    const float cameraSpeed = 0.5f * deltaTime; // adjust accordingly

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // user input
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT, deltaTime);
        //cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        //zRotation -= rotStep * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT, deltaTime);
        //cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        //zRotation += rotStep * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD, deltaTime);
        /*yTranslate += sin(glm::radians(zRotation)) * transStep * deltaTime;
        xTranslate += cos(glm::radians(zRotation)) * transStep * deltaTime;*/
        //cameraPos += cameraSpeed * cameraFront;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        //cameraPos -= cameraSpeed * cameraFront;
        /*yTranslate -= sin(glm::radians(zRotation)) * transStep * deltaTime;
        xTranslate -= cos(glm::radians(zRotation)) * transStep * deltaTime;*/
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    gl3::shader litShader = gl3::shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
    gl3::shader lightShader = gl3::shader("shaders/vertexShader.glsl", "shaders/lightFragmentShader.glsl");;

    // Space Ship vertices
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    files::texture textureInfoContainerDiffuse = files::FileManager::loadTextureFromFile("container2.png");
    files::texture textureInfoContainerSpecular = files::FileManager::loadTextureFromFile("container2_specular.png");
    files::texture textureInfoContainerEmission = files::FileManager::loadTextureFromFile("matrix.jpg");

    //Vertex Buffer Object
    unsigned int VBO,VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int diffuseMap,specularMap,emissionMap;
    glGenTextures(1, &diffuseMap);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    if(textureInfoContainerDiffuse.content){
        GLenum format;
        if (textureInfoContainerDiffuse.colChannel == 1)
            format = GL_RED;
        else if (textureInfoContainerDiffuse.colChannel == 3)
            format = GL_RGB;
        else if (textureInfoContainerDiffuse.colChannel == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureInfoContainerDiffuse.width, textureInfoContainerDiffuse.height, 0,
                     format, GL_UNSIGNED_BYTE, textureInfoContainerDiffuse.content);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }else{
        std::cout << "Failed to load diffuseMap" << std::endl;
    }
    stbi_image_free(textureInfoContainerDiffuse.content);

    glGenTextures(1, &specularMap);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    if(textureInfoContainerSpecular.content){
        GLenum format;
        if (textureInfoContainerSpecular.colChannel == 1)
            format = GL_RED;
        else if (textureInfoContainerSpecular.colChannel == 3)
            format = GL_RGB;
        else if (textureInfoContainerSpecular.colChannel == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureInfoContainerSpecular.width, textureInfoContainerSpecular.height, 0,
                     format, GL_UNSIGNED_BYTE, textureInfoContainerSpecular.content);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }else{
        std::cout << "Failed to load specularMap" << std::endl;
    }
    stbi_image_free(textureInfoContainerSpecular.content);

    glGenTextures(1, &emissionMap);
    glBindTexture(GL_TEXTURE_2D, emissionMap);
    if(textureInfoContainerEmission.content){
        GLenum format;
        if (textureInfoContainerEmission.colChannel == 1)
            format = GL_RED;
        else if (textureInfoContainerEmission.colChannel == 3)
            format = GL_RGB;
        else if (textureInfoContainerEmission.colChannel == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureInfoContainerEmission.width, textureInfoContainerEmission.height, 0,
                     format, GL_UNSIGNED_BYTE, textureInfoContainerEmission.content);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }else{
        std::cout << "Failed to load emissionMap" << std::endl;
    }
    stbi_image_free(textureInfoContainerEmission.content);


    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    // we only need to bind to the VBO, the container's VBO's data already contains the data.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // set the vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    litShader.use();
    litShader.setInt("material.diffuse", 0);
    litShader.setInt("material.specular", 1);
    litShader.setInt("material.emission", 2);

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    glEnable(GL_DEPTH_TEST);

    glfwSetTime(1.0/60);

    std::cout << pointLightPositions->length() << std::endl;

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processUserInput(window);


        litShader.use();

        litShader.setVector("viewPos",glm::vec4(camera.Position, 1.0f));

        litShader.setVector3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

        litShader.setVector3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        litShader.setVector3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        litShader.setVector3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

        for(int i = 0; i < 4; i++){
            litShader.setVector3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);

            litShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            litShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.9f);
            litShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);

            litShader.setVector3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
            litShader.setVector3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            litShader.setVector3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        }

        litShader.setVector3("spotLight.direction", camera.Front);
        litShader.setVector3("spotLight.position", camera.Position);

        litShader.setFloat("spotLight.constant", 1.0f);
        litShader.setFloat("spotLight.linear", 0.09f);
        litShader.setFloat("spotLight.quadratic", 0.032f);

        litShader.setVector3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        litShader.setVector3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        litShader.setVector3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        litShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
        litShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));

        // material properties
        litShader.setVector("material.specular",glm::vec4(0.5f, 0.5f, 0.5f,1.0f)); // specular lighting doesn't have full effect on this object's material
        litShader.setFloat("material.shininess", 64.0f);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind diffuse map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, emissionMap);

        glm::mat4 view;
        view = camera.GetViewMatrix();
        litShader.setMatrix("view", view);
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.Zoom), W_WIDTH / W_HEIGHT, 0.1f, 100.0f);
        litShader.setMatrix("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        glBindVertexArray(VAO);

        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            litShader.setMatrix("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        lightShader.use();
        view = camera.GetViewMatrix();
        litShader.setMatrix("view", view);
        projection = glm::perspective(glm::radians(camera.Zoom), W_WIDTH / W_HEIGHT, 0.1f, 100.0f);
        litShader.setMatrix("projection", projection);
        lightShader.setMatrix("view", view);
        lightShader.setMatrix("projection", projection);

        glBindVertexArray(lightVAO);

        for(int i = 0; i < 4; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMatrix("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }




        // update deltaTime
        deltaTime = glfwGetTime();
        glfwSetTime(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void modelTransform(gl3::shader* shaderProgram){//unsigned int shaderProgram){

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    //model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

    shaderProgram->setMatrix("model", model);

    /*int modelLocation = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));*/
}

void viewTransform(gl3::shader* shaderProgram){//unsigned int shaderProgram){

    glm::mat4 view  = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    /*view = glm::lookAt(glm::vec3(0.0f,0.0f,25.0f),
                       glm::vec3(0.0f,0.0f,0.0f),
                       glm::vec3(0.0f,1.0f,0.0f));/*glm::vec3(0.0f,0.0f,0.0f),
                       glm::vec3(0.0f,0.0f,-1.0f),
                       glm::vec3(0.0f,1.0f,0.0f));*/

    shaderProgram->setMatrix("view",view);

    /*int viewLocation = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));*/
}

void projectionTransform(gl3::shader* shaderProgram){//unsigned int shaderProgram){

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), W_WIDTH/W_HEIGHT, 0.1f, 100.0f);

    shaderProgram->setMatrix("projection", projection);

    /*int projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));*/
}