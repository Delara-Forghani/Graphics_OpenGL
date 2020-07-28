#include "Camera.h"
#include "iostream"


float pitch = 0.0f;
float yaw = -90.0f;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

bool reverseYaw = false;
bool reversePitch = false;
Camera::Camera(short width, short height) :
	isPerspective(true),
	fieldOfView(45.f),
	nearDistance(0.1f),
	farDistance(100.f)
{
	this->width = width;
	this->height = height;
	ComputeViewMatrix();
	ComputeProjectionMatrix();
}

void Camera::Update(GLFWwindow* window)
{
	static double lastX = 0, lastY = 0;
	static float rotX = 0.f, rotY = 0.f;
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	double deltaX = x - lastX;
	double deltaY = y - lastY;
	lastX = x;
	lastY = y;

	auto targetY = (float)(deltaY / 50);
	auto targetX = (float)(deltaX / 50);

	auto forward = GetForward();
	auto right = GetRight();
	auto rotation = glm::eulerAngles(GetQuaternion());

	//std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
	//std::cout << forward.x << " " << forward.y << " " << forward.z << std::endl;
	//std::cout << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;
	
	forward.z = -forward.z;
	right.z = -right.z;
	auto pos = GetPosition();

	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	std::cout <<"frame rate: "<< deltaTime << std::endl;
	lastFrame = currentFrame;

	float cameraSpeed = 2.f * deltaTime; // adjust accordingly
	//camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //move forward
	{
		SetPosition(pos + (forward*cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //move backward
	{
		SetPosition(pos - (forward*cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //move rightward
	{
		SetPosition(pos - (right*cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //move leftward
	{
		SetPosition(pos + (right*cameraSpeed));
	}if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) //move upward
	{
		SetPosition(pos - (glm::cross(forward,right) * cameraSpeed));  
	}if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) //move downward
	{
		SetPosition(pos + (glm::cross(forward, right) * cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
	{
		SetPosition(pos + ((forward - right)*cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		SetPosition(pos + ((forward + right)*cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		SetPosition(pos +((- forward - right)*cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		SetPosition(pos +((- forward + right)*cameraSpeed));
	}	
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		SetPosition(pos + ((-glm::cross(forward, right) - right) * cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		SetPosition(pos + ((-glm::cross(forward, right) + right) * cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		SetPosition(pos + ((glm::cross(forward, right) - right) * cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		SetPosition(pos + ((glm::cross(forward, right) + right) * cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		SetPosition(pos + ((-glm::cross(forward, right) + forward) * cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		SetPosition(pos + ((-glm::cross(forward, right) - forward) * cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		SetPosition(pos + ((glm::cross(forward, right) + forward) * cameraSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		SetPosition(pos + ((glm::cross(forward, right) - forward) * cameraSpeed));
	}


	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) //change yaw direction
	{
		if (!reverseYaw) {
			reverseYaw = true;
		}
		else {
			reverseYaw = false;
		}
	}if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) //change pitch direction
	{
		if (!reversePitch) {
			reversePitch = true;
		}
		else {
			reversePitch = false;
		}
	}

	if (!reverseYaw) {
		rotX -= targetX; //direction.x
	}else if (reverseYaw) {
		rotX += targetX; //direction.x
	}

	if (!reversePitch) {
		rotY -= targetY; //direction.y
	}else if (reversePitch) {
		rotY += targetY; //direction.y
	}
	
	
	rotY = glm::clamp<float>(rotY, -180, 180); //retriction over angle 
	

	//rotate camera
	auto q = glm::angleAxis(glm::radians(rotX), glm::vec3(0.f, 1.f, 0.f))*glm::angleAxis(glm::radians(rotY), glm::normalize(GetRight()));	
	SetQuaternion(q);
	
	

}

void Camera::Setup()
{

}

void Camera::SetupMatrices()
{
	PositionedOrientedObject::SetupMatrices();
	ComputeViewMatrix();
}

void Camera::ComputeViewMatrix()
{
	viewMatrix = glm::mat4(RotateWithRadianAngles(glm::mat4(1.f), -glm::degrees(glm::eulerAngles(GetQuaternion()))));
	viewMatrix = glm::translate(viewMatrix, -GetPosition());
}

void Camera::ComputeProjectionMatrix()
{
	if (isPerspective)
	{
		projectionMatrix = glm::perspective(glm::radians(fieldOfView), 1.f * width / height, nearDistance, farDistance);
	}
	else
	{
		projectionMatrix = glm::ortho(0.f, (float)width, 0.f, (float)height, nearDistance, farDistance);
	}
}

glm::mat4x4 Camera::GetViewMatrix()
{
	return this->viewMatrix;
}

glm::mat4x4 Camera::GetProjectionMatrix()
{
	return this->projectionMatrix;
}

void Camera::SetViewPortSize(short width, short height, bool shouldCompute = true)
{
	this->width = width;
	this->height = height;
	if (shouldCompute)
		ComputeProjectionMatrix();
}

void Camera::SetPerspective(bool prs, bool shouldCompute = true)
{
	this->isPerspective = prs;
	if (shouldCompute)
		ComputeProjectionMatrix();
}

void Camera::SetFieldOfView(float fov, bool shouldCompute = true)
{
	this->fieldOfView = fov;
	if (shouldCompute)
		ComputeProjectionMatrix();
}

void Camera::SetNearDistance(float near, bool shouldCompute = true)
{
	this->nearDistance = near;
	if (shouldCompute)
		ComputeProjectionMatrix();
}

void Camera::SetFarDistance(float far, bool shouldCompute = true)
{
	this->farDistance = far;
	if (shouldCompute)
		ComputeProjectionMatrix();
}

bool Camera::IsPerspective()
{
	return isPerspective;
}

void Camera::GetViewPortSize(short* width, short* height)
{
	width = &this->width;
	height = &this->height;
}

float Camera::GetFieldOfView()
{
	return this->fieldOfView;
}

float Camera::GetNearDistance()
{
	return this->nearDistance;
}

float Camera::GetFarDistance()
{
	return this->farDistance;
}


