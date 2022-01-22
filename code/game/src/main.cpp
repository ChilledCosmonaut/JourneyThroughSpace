#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "../FileManager/stb_image.h"


double deltaTime;

const float W_WIDTH = 800.0f;
const float W_HEIGHT = 600.0f;
const char* W_TITLE = "GameLab III";

float zRotation = 0.0f;
float rotStep = 90.0f;
float xTranslate = 0.0f, yTranslate = 0.0f;
float transStep = 1.0f;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float yaw = -90.0f, pitch = 0.0f, fov = 45.0f;

float lastX = 400, lastY = 300;

const char* vShaderSource = "#version 460 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "layout (location = 1) in vec3 aColor;\n"
                            "uniform mat4 model;\n"
                            "uniform mat4 view;\n"
                            "uniform mat4 projection;\n"
                            "out vec3 ourColor;\n"
                            "void main()\n"
                            "{\n"
                            "    ourColor = aColor;\n"
                            "    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                            "}\n";

const char* fShaderSource = "#version 460 core\n"
                            "out vec4 fragColor;\n"
                            "in vec3 ourColor;\n"
                            "void main()\n"
                            "{\n"
                            "   fragColor = vec4(ourColor, 1.0f);\n"
                            "}\n";

const char* fShaderSource2 = "#version 460 core\n"
                            "out vec4 fragColor;\n"
                            "uniform vec4 color;\n"
                             "in vec3 ourColor;\n"
                            "void main()\n"
                            "{\n"
                            "   fragColor = color;\n"
                            "}\n";


void modelTransform(gl3::shader* shaderProgram);//unsigned int shaderProgram);
void viewTransform(gl3::shader* shaderProgram);//unsigned int shaderProgram);
void projectionTransform(gl3::shader* shaderProgram);//unsigned int shaderProgram);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
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
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        //zRotation -= rotStep * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        //zRotation += rotStep * deltaTime;
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        /*yTranslate += sin(glm::radians(zRotation)) * transStep * deltaTime;
        xTranslate += cos(glm::radians(zRotation)) * transStep * deltaTime;*/
        cameraPos += cameraSpeed * cameraFront;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cameraPos -= cameraSpeed * cameraFront;
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

    // Fragment Shader
    unsigned int fShader2;
    fShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader2, 1, &fShaderSource2, nullptr);
    glCompileShader(fShader2);

    glGetShaderiv(fShader2, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fShader2, 512, nullptr, infoLog);
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
    }

    gl3::shader shader = gl3::shader("shaders/vertexShader.glsl","shaders/fragmentShader.glsl");
    shader.use();

    // Space Ship vertices
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };



    files::texture textureInfoWall = files::FileManager::loadTextureFromFile("wall.jpg");
    files::texture textureInfoFace = files::FileManager::loadTextureFromFile("awesomeface.png");

    //Vertex Buffer Object
    unsigned int VBO,VAO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture1,texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture1 wrapping/filtering options (on the currently bound texture1 object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(textureInfoWall.content){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureInfoWall.width, textureInfoWall.height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, textureInfoWall.content);
    glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(textureInfoWall.content);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture1 wrapping/filtering options (on the currently bound texture1 object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(textureInfoFace.content){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureInfoFace.width, textureInfoFace.height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, textureInfoFace.content);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(textureInfoFace.content);
    // color attribute

    shader.setInt("texture1",0);
    shader.setInt("texture2",1);


    //Element Buffer Object
    /*unsigned int EBO;
    glGenBuffers(1, &EBO);*/


    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    glEnable(GL_DEPTH_TEST);

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

    glfwSetTime(1.0/60);

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processUserInput(window);

        //shader.setMatrix("model",trans);
        //modelTransform(&shader);
        //viewTransform(&shader);

        const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        glm::mat4 view;
        //view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0,0.0));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMatrix("view", view);
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
        shader.setMatrix("projection", projection);
        //projectionTransform(&shader);
        shader.setFloat("mixingFactor", xTranslate);

        try {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
        }catch (std::exception exception){
            std::cout << exception.what() << std::endl;
        }


        glBindVertexArray(VAO);

        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMatrix("model", model);

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