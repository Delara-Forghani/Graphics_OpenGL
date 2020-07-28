//#include "pch.h"

#include "CubeObject.h"
#include "stb_image.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


CubeObject::CubeObject(GraphicDevice* device) : GameObject(device)
{
	float v[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

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
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	unsigned int ii[] = {
		0, 1, 2,
		3, 4, 5,

		6, 7, 8,
		9, 10, 11,

		12, 13, 14,
		15, 16, 17,

		18, 19, 20,
		21, 22, 23,

		24, 25, 26,
		27, 28, 29,

		30, 31, 32,
		33, 34, 35
	};

	verticesSize = 180;
	indicesSize = 36;
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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

void CubeObject::Update(GLFWwindow* window)
{
	if (selected) {
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)  //positive rotation over y axis
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(0.25f), glm::vec3(0.f, 1.f, 0.f)));
		}
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) //negative rotation over y axis
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(-0.25f), glm::vec3(0.f, 1.f, 0.f)));
		}if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)//positive rotation over x axis
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(0.25f), glm::vec3(1.f, 0.f, 0.f)));
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) //negative rotation over x axis
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(-0.25f), glm::vec3(1.f, 0.f, 0.f)));
		}if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) //positive rotation over z axis
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(0.25f), glm::vec3(0.f, 0.f, 1.f)));
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) //negative rotation over z axis
		{
			auto angle = GetQuaternion();
			SetQuaternion(glm::rotate(angle, glm::radians(-0.25f), glm::vec3(0.f, 0.f, 1.f)));
		}if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) //positive scale along x axis
		{
			glm::vec3 currentScale = GetScale();
			SetScale(glm::vec3(currentScale.x + 0.001, currentScale.y, currentScale.z));

		}if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) //negative scale along x axis
		{
			glm::vec3 currentScale = GetScale();
			if (currentScale.x > 0) {
				//std::cout << currentScale.x << std::endl;
				SetScale(glm::vec3(currentScale.x - 0.001, currentScale.y, currentScale.z));
			}

		}if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) //positive scale along y axis
		{
			glm::vec3 currentScale = GetScale();
			SetScale(glm::vec3(currentScale.x, currentScale.y + 0.001, currentScale.z));

		}if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) //negative scale along y axis
		{

			glm::vec3 currentScale = GetScale();
			if (currentScale.y > 0) {
				//std::cout << currentScale.y << std::endl;
				SetScale(glm::vec3(currentScale.x, currentScale.y - 0.001, currentScale.z));
			}

		}if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)//positive scale along z axis
		{
			glm::vec3 currentScale = GetScale();
			SetScale(glm::vec3(currentScale.x, currentScale.y, currentScale.z + 0.001));

		}if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)//negative scale along z axis
		{

			glm::vec3 currentScale = GetScale();
			if (currentScale.z > 0) {
				//std::cout << currentScale.z << std::endl;
				SetScale(glm::vec3(currentScale.x, currentScale.y, currentScale.z - 0.001));
			}

		}
	}


}

void CubeObject::Setup()   //initial state
{
	float xScale1 = 0.5;
	float yScale1 = 0.5;
	float zScale1 = 0.5;
	SetPosition(glm::vec3(0, 0, -6));
	SetScale(glm::vec3(xScale1, yScale1, zScale1));
	SetQuaternion(glm::rotate(RotateWithRadianAngles(glm::mat4(), glm::vec3(0.0f, 0.0f, 1.0f)), glm::radians(0.f), glm::vec3(0.0f, 0.0f, 1.0f)));
}
