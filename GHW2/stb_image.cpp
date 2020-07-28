#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <iostream>
#include <stdlib.h> 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
//callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
//Transformation parameters
float xScale = 1;
float yScale = 1;
float rotate = 0;
//flags to set the state
bool yExpand = false;
bool yCompress = false;
bool xCompress = false;
bool xExpand = false;
bool clkWise = false;
bool cntClkWise = false;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;	
	}

	glfwSetKeyCallback(window, keyCallback);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("texture.vs", "texture.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// load and create a texture 
	// -------------------------
	unsigned int texture1;
	unsigned int texture2;

	glGenTextures(1, &texture1);
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
  // ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("awesometree.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesometree.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use(); //activate/use the shader before setting uniforms!
	// either set it manually like so:
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//identity matrix
		glm::mat4 trans = glm::mat4(1.0f);
		//find our Uniform from shaders (Uniforms are global variables)
		int colorCoeff = glGetUniformLocation(ourShader.ID, "coeff");
		// render container
		ourShader.use();
		//set the value of the uniform
		glUniform3f(colorCoeff, 1.0f, 1.0f, 1.0f);
		//check the state of the command
		if (yExpand) {
			// scale matrix should always come before rotation matrix due to correct matrix multiplication
			yScale = yScale + 0.001;
			trans = glm::scale(trans, glm::vec3(xScale, yScale, 1.0));
			trans = glm::rotate(trans, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (yCompress) {
			if (yScale > 0.001) {
				yScale = yScale - 0.001;
			}
			trans = glm::scale(trans, glm::vec3(xScale, yScale, 1.0));
			trans = glm::rotate(trans, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (xCompress) {
			if (xScale > 0.001) {
				xScale = xScale - 0.001;
			}
			trans = glm::scale(trans, glm::vec3(xScale, yScale, 1.0));
			trans = glm::rotate(trans, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (xExpand) {
			xScale = xScale + 0.001;
			trans = glm::scale(trans, glm::vec3(xScale, yScale, 1.0));
			trans = glm::rotate(trans, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else if (clkWise) {
			trans = glm::scale(trans, glm::vec3(xScale, yScale, 1.0));
			trans = glm::rotate(trans, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
			//changing color intensity during rotation
			float redVal = (float)rand() / RAND_MAX;
			float greenVal = (float)rand() / RAND_MAX;
			float blueVal = (float)rand() / RAND_MAX;
			int colorCoeff = glGetUniformLocation(ourShader.ID, "coeff");
			// render container
			ourShader.use();
			glUniform3f(colorCoeff, redVal, greenVal, blueVal);
			rotate = rotate - 0.001;
		}
		else if (cntClkWise) {
			trans = glm::scale(trans, glm::vec3(xScale, yScale, 1.0));
			trans = glm::rotate(trans, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
			float redVal = (float)rand() / RAND_MAX;
			float greenVal = (float)rand() / RAND_MAX;
			float blueVal = (float)rand() / RAND_MAX;
			int colorCoeff = glGetUniformLocation(ourShader.ID, "coeff");
			// render container
			ourShader.use();
			glUniform3f(colorCoeff, redVal, greenVal, blueVal);
			rotate = rotate + 0.001;
		}

		// render container
		ourShader.use();

		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

//command 
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		std::cout << "w key pressed" << std::endl;
		yExpand = true;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		std::cout << "s key pressed" << std::endl;
		yExpand = false;
		yCompress = true;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		std::cout << "A key pressed" << std::endl;
		yExpand = false;
		yCompress = false;
		xCompress = true;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		std::cout << "D key pressed" << std::endl;
		yExpand = false;
		yCompress = false;
		xCompress = false;
		xExpand = true;
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		std::cout << "Q key pressed" << std::endl;
		yExpand = false;
		yCompress = false;
		xCompress = false;
		xExpand = false;
		clkWise = true;

	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		std::cout << "Q key pressed" << std::endl;
		yExpand = false;
		yCompress = false;
		xCompress = false;
		xExpand = false;
		clkWise = false;
		cntClkWise = true;

	}
}