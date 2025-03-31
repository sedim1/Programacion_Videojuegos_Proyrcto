#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"
#include "Entity.h"
#include "Scene3D.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void viewportInput(GLFWwindow* window);
bool Init();
Scene3D SetUpScene3D();
void Finish();
void Update(Scene3D& scene);

using namespace glm;

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 840;
float lastX = (float)SCR_WIDTH / 2;
float lasty = (float)SCR_HEIGHT / 2;
GLFWwindow* window;

//Shaders
Shader objectShader;

//Cameras
Camera viewport;
Camera inGame;
Camera* current;
bool firstMouse = true;
//Scene3D
Scene3D* currentScene;

//Delta time
float deltaTime = 0.0f;

int main()
{
    //Start glfw, opengl and 
    if (!Init())
        return -1;
    Scene3D gameScene = SetUpScene3D();
    currentScene = &gameScene;
    Update(gameScene);
    Finish();
    return 0;
}

bool Init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PROYECTO_PROGRAMACION_VIDEOJUEGOS", NULL, NULL);

    // glfw window creation
    // --------------------

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //Set events to detect
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    return true;
}

Scene3D SetUpScene3D() {
    Scene3D gameScene;
    //Start shaders
    objectShader = Shader("SHADERS\\Primitives\\vertexShader.vs", "SHADERS\\Primitives\\fragmentShader.fs");
    //SetUp Scene
    //SetUp vieweport camera
    viewport.w = SCR_WIDTH;
    viewport.h = SCR_HEIGHT;
    current = &viewport;
    //SetUp inGAME Camera
    inGame = Camera(vec3(0.0f, 20.0f, 3.0f),vec3(0.0f, 1.0f, 0.0f), -90.0f, -55.0f);
    inGame.w = SCR_WIDTH;
    inGame.h = SCR_HEIGHT;
    Player* player = new Player();
    MapPlane* map = new MapPlane();
    player->setShader(objectShader);
    map->setShader(objectShader);
    gameScene = Scene3D(viewport,inGame);
    gameScene.addObjectToScene(player);
    gameScene.addObjectToScene(map);
    return gameScene;
}

void Update(Scene3D& gameScene) {
    float currentTime = 0.0f;
    float lastTime = 0.0f;
    
    while (!glfwWindowShouldClose(window))
    {
        //Calculate deltaTime
        currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        // input
        processInput(window);
        gameScene.viewportInput(window,deltaTime);
        //Process logic
        gameScene.processEntities(deltaTime);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gameScene.drawEntities();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Finish() {
    glDeleteProgram(objectShader.ID);
    glfwTerminate();
}

void processInput(GLFWwindow* window)
{
    //Window Input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        currentScene->enableViewport = true;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        currentScene->enableViewport = false;
}

void viewportInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        viewport.ProcessKeyboard(FORWARD,deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        viewport.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        viewport.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        viewport.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    viewport.h = height;
    viewport.w = width;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lasty = ypos;
        firstMouse = false;
    }
    float dx = xpos - lastX;
    float dy = lasty - ypos;
    lastX = xpos;
    lasty = ypos;
    currentScene->viewportCamera.ProcessMouseMovement(dx,dy);
}