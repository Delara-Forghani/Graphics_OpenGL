#include "PyramidObject.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
PyramidObject::PyramidObject(GraphicDevice* device) : GameObject(device)
{
	float v[] = {
		//Triangle1
		 0.0f, 0.5f, 0.0f, 
		 1.f, 0.0f, 0.0f, 

		 -0.5f, -0.5f, 0.5f, 
		  0.0f, 1.f, 0.0f,

		 0.5f, -0.5f,  0.5f, 
		 0.0f,  0.0f,  1.f,

		 //Triangle2
		  0.0f,	0.5f,  0.0f,
		  1.f, 0.0f,  0.0f,

		  0.5f, -0.5f, 0.5f,
		  0.0f,  1.f, 0.0f,

		  0.5f, -0.5f, -0.5f,
		  0.0f,  0.0f, 1.f,

		  //Triangle3
		   0.0f,  0.5f,  0.0f,
		  1.f,  0.0f,  0.0f,

		   0.5f, -0.5f, -0.5f,
		   0.0f,  1.f,  0.0f,

		   -0.5f, -0.5f,  -0.5f,
		   0.0f,  0.0f,  1.f,

		   //Triangle4
		   0.0f,	0.5f,  0.0f,
		   1.f,  0.0f,  0.0f,

		   -0.5f, -0.5f, -0.5f,
		   0.0f,  1.f,  0.0f,

		   -0.5f, -0.5f,  0.5f,
		   0.0f,  0.0f, 1.f,

		   //Bottom1
		  -0.5f, -0.5f,  0.5f,
		  1.f,  0.0f,  0.0f,

		  0.5f, -0.5f, 0.5f,
		  0.0f,  1.f,  0.0f,

		  -0.5f, -0.5f, -0.5f,
		  0.0f,  0.0f,  1.f,

		  //Bottom2
		   -0.5f, -0.5f,  -0.5f,
		   1.f,  0.0f,  0.0f,

		   0.5f, -0.5f, -0.5f,
		   0.0f,  1.f,  0.0f,

		   0.5f, -0.5f, 0.5f,
		   0.0f,  0.0f, 1.f
	};
	unsigned int ii[] = {
		//Triangle1
		0, 1, 2,
		//Triangle2
		3, 4, 5,
		//Triangle3
		6, 7, 8,
		//Triangle4
		9, 10, 11,
		//bottom1
		12, 14, 13,
		//bottom2
		15, 16, 17
	};

	verticesSize = 108;
	indicesSize = 18;
	vertices = new float[verticesSize];
	indices = new unsigned int[indicesSize];
	for (size_t i = 0; i < verticesSize; i++)
	{
		vertices[i] = v[i];
	}
	for (size_t i = 0; i < indicesSize; i++)
	{
		indices[i] = ii[i];
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
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
}

void PyramidObject::Update(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		selected = true;
	}
	if (selected) {
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(0.25f), glm::vec3(0.f, 1.f, 0.f)));
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(-0.25f), glm::vec3(0.f, 1.f, 0.f)));
		}if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(0.25f), glm::vec3(1.f, 0.f, 0.f)));
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(-0.25f), glm::vec3(1.f, 0.f, 0.f)));
		}if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(0.25f), glm::vec3(0.f, 0.f, 1.f)));
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(-0.25f), glm::vec3(0.f, 0.f, 1.f)));
		}if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
			glm::vec3 currentScale = GetScale();
			SetScale(glm::vec3(currentScale.x + 0.001, currentScale.y, currentScale.z));

		}if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			glm::vec3 currentScale = GetScale();
			if (currentScale.x > 0) {
				//std::cout << currentScale.x << std::endl;
				SetScale(glm::vec3(currentScale.x - 0.001, currentScale.y, currentScale.z));
			}

		}if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
			glm::vec3 currentScale = GetScale();
			SetScale(glm::vec3(currentScale.x, currentScale.y + 0.001, currentScale.z));

		}if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{

			glm::vec3 currentScale = GetScale();
			if (currentScale.y > 0) {
				//std::cout << currentScale.y << std::endl;
				SetScale(glm::vec3(currentScale.x, currentScale.y - 0.001, currentScale.z));
			}

		}if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{
			glm::vec3 currentScale = GetScale();
			SetScale(glm::vec3(currentScale.x, currentScale.y, currentScale.z + 0.001));

		}if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		{

			glm::vec3 currentScale = GetScale();
			if (currentScale.z > 0) {
				//std::cout << currentScale.z << std::endl;
				SetScale(glm::vec3(currentScale.x, currentScale.y, currentScale.z - 0.001));
			}

		}
	}

}

void PyramidObject::Setup()
{
	float xScale = 1;
	float yScale = 1;
	float zScale = 1;
	//SetPosition(glm::vec3(0, 0, -3));
	SetScale(glm::vec3(xScale, yScale, zScale));
	SetQuaternion(glm::rotate(RotateWithRadianAngles(glm::mat4(), glm::vec3(0.0f, 0.0f, 1.0f)), glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f)));
}
