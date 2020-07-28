#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//This method adjusts the window size whenever user resizes it
void framebuffer_size_callback(GLFWwindow* window, int w, int h);
//The function returns whether this key is currently being pressed
void processInput(GLFWwindow* window);

// settings
unsigned int width = 800;
unsigned int height = 600;

// Vertex Buffer object ID
unsigned int VBO;
// Vertex Array object ID
unsigned int VAO;
//Element buffer object ID
unsigned int EBO;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
unsigned int vertexShader;

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(72.0f/255.0f, 48.0f/255.0f, 77.0f/255.0f, 1.0f);\n"
"}\0";
unsigned int fragmentShader;

unsigned int shaderProgram;


//find positionof the cursor (just for test)
static void cursorPositionCallback(GLFWwindow *window, double xPos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

int choosedVertex = -1;
bool fillCheck = true;


int main()
{   //window options setting
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //create main window
    GLFWwindow* window = glfwCreateWindow(width, height, "mainWindow" , NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //Event handlers 
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //vertices array
    float vertices[] = {
           0.5f,  0.5f, 0.0f,  // top right
           0.5f, -0.5f, 0.0f,  // bottom right
           -0.5f, -0.5f, 0.0f,  // bottom left
           -0.5f,  0.5f, 0.0f   // top left 
    };

    //render loop
    while (!glfwWindowShouldClose(window))
    {
        //input check
        processInput(window);

        //rendering
        glClearColor(233.0f/255.0f, 209.0f/255.0f, 229.0f/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

       
        unsigned int indices[] = {  //we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

      //check the vertex index being dragged
        if (choosedVertex == 0) {
            //get cursor position
            double posX, posY;
            glfwGetCursorPos(window, &posX, &posY);
            //coordinates conversion to the interval [-1.0f,1.0f]
            float x = -1.0f + 2 * posX / width;
            float y = +1.0f - 2 * posY / height;
            //vertex number 0
            vertices[0] = x;
            vertices[1] = y;
        }else if (choosedVertex == 1) {
            //get cursor position
            double posX, posY;
            glfwGetCursorPos(window, &posX, &posY);
            //coordinates conversion to the interval [-1.0f,1.0f]
            float x = -1.0f + 2 * posX / width;
            float y = +1.0f - 2 * posY / height;
            //vertex number 0
            vertices[3] = x;
            vertices[4] = y;
        }else if (choosedVertex == 2) {
            //get cursor position
            double posX, posY;
            glfwGetCursorPos(window, &posX, &posY);
            //coordinates conversion to the interval [-1.0f,1.0f]
            float x = -1.0f + 2 * posX / width;
            float y = +1.0f - 2 * posY / height;
            //vertex number 0
            vertices[6] = x;
            vertices[7] = y;
        }if (choosedVertex == 3) {
            //get cursor position
            double posX, posY;
            glfwGetCursorPos(window, &posX, &posY);
            //coordinates conversion to the interval [-1.0f,1.0f]
            float x = -1.0f + 2 * posX / width;
            float y = +1.0f - 2 * posY / height;
            //vertex number 0
            vertices[9] = x;
            vertices[10] = y;
        }
        //flag for state conversion between fill and wireframe 
        if (fillCheck) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        //generate buffer
        glGenBuffers(1, &VBO);

        glGenBuffers(1, &EBO);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        //bind vertex buffer to GL_ARRAY_BUFFER
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
       
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        

        

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        

        //ckeck and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
//resize the window
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//this is for test
static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos){
    //std::cout << xpos << " : " << ypos<< std::endl;
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double posX, posY;
        glfwGetCursorPos(window, &posX, &posY);
        std::cout << "(x,y) coordinates are: " << posX << " " << posY << std::endl;

        float x = -1.0f + 2 * posX / width;
        float y = +1.0f - 2 * posY / height;
        
        std::cout << "X: " << x << std::endl;
        std::cout << "Y: " << y << std::endl;

        // threshold in order to discern the clicked vertex 
        int min = 0.25;
        if (pow((x - 0.5f), 2) + pow((y - 0.5f), 2) < 0.25f) {
            choosedVertex = 0;
        }else if (pow((x - 0.5f), 2) + pow((y + 0.5f), 2) < 0.25f) {
            choosedVertex = 1;
        }
        else if (pow((x + 0.5f), 2) + pow((y + 0.5f), 2) < 0.25f) {
            choosedVertex = 2;
        }
        else if (pow((x + 0.5f), 2) + pow((y - 0.5f), 2) < 0.25f) {
            choosedVertex = 3;
        }
        
        std::cout << choosedVertex << std::endl;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        choosedVertex = -1;
        //std::cout << "LEFT button released" << std::endl;
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        std::cout << "space key pressed" << std::endl;
        if (fillCheck) {
            fillCheck = false;
        }
        else
        {
            fillCheck = true;
        }
    }
}
