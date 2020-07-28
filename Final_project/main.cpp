#include <iostream>

//#include "pch.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GameObject.h"
#include "GraphicDevice.h"
#include "CubeObject.h"
#include "PyramidObject.h"
#include "PrismObject.h"
#include "Camera.h"


using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void update(GLFWwindow* window);
void render();
void init();

// Screen dimansion
unsigned int width = 1920, height = 1080;

// App globals
GraphicDevice device;
vector<GameObject*> objects;
Camera* camera;


int main(int argc, char** argv)
{

    // Initializing window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    init();

    while (!glfwWindowShouldClose(window))
    {
        update(window);

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void init()
{

    device.SetupDevice();
    camera = new Camera(width, height);


    auto cube = new CubeObject(&device);  
    cube->Setup();
    objects.push_back(cube);

    auto pyram1 = new PyramidObject(&device);
    pyram1->Setup();  
    objects.push_back(pyram1);


    auto prism = new PrismObject(&device);
    prism->Setup();
    objects.push_back(prism);



    camera->Setup();
   
    for (auto object = objects.begin(); object != objects.end(); object++)
    {
        (*object)->Render();
    }
}

void update(GLFWwindow* window)
{
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    camera->Update(window);

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {  //render new cube in front of the camera
        auto cube = new CubeObject(&device);
        cube->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
        cube->SetPosition(camera->GetForward() + glm::vec3(0.f, 0.f, -2.f) + camera->GetPosition());
        cube->selected = true;
        objects.push_back(cube);

    }if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) { //render new pyramid in front of the camera
        auto pyram = new PyramidObject(&device);
        pyram->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
        pyram->SetPosition(camera->GetForward() + camera->GetPosition()+ glm::vec3(0.f, 0.f, -2.f));
        pyram->selected = true;
        objects.push_back(pyram);

    }if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) { //render new prism in front of the camera
        auto prism = new PrismObject(&device);
        prism->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
        prism->SetPosition(camera->GetForward() + camera->GetPosition()+glm::vec3(0.f, 0.f, -4.f));
        prism->selected = true;
        objects.push_back(prism);

    }if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { //exchange from normal state to focus state and vice versa
        for (auto object = objects.begin(); object != objects.end(); object++) {
            if ((*object)->selected) {
                (*object)->selected = false;
            }
        }
    }

    
   

    for (auto object = objects.begin(); object != objects.end(); object++)
    {
        //int index = distance(objects.begin(),object);
        //std::cout << "index: " <<index << std::endl;
        (*object)->Update(window);
        //if (abs((*object)->GetPosition().z - camera->GetPosition().z) < 4 && abs((*object)->GetPosition().x - camera->GetPosition().x) < 4 &&
        //    abs((*object)->GetPosition().y - camera->GetPosition().y) < 4) {
        //    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        //        (*object)->selected = true;
        //        }
       // }
        if ((*object)->selected) {
            (*object)->SetPosition(camera->GetForward() +glm::vec3(0.f,0.f,-3.f)+ camera->GetPosition()); //place an object in front of the camera
        }
        
    }
}

void render()
{
    unsigned int projLoc = glGetUniformLocation(device.GetShader()->ID, "projection");
    unsigned int viewLoc = glGetUniformLocation(device.GetShader()->ID, "view");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    device.Render(&objects);

}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
    camera->SetViewPortSize(w, h, true);
    width = w;
    height = h;
}

