// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "Input.hpp"
//#define DEBUG_PRINT

[[nodiscard]] glm::mat4 Camera::RetrieveLookAt() const
{
	return glm::lookAt(CameraPosition, CameraPosition + LookAt, CameraUpwardVector);
}

void Camera::ProcessKeyboard(GLFWwindow* window, float deltaTime)
{
	const float speedFactor = 3.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		CameraPosition += LookAt * speedFactor;
#ifdef DEBUG_PRINT
		DebugPrint();
#endif
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		CameraPosition -= LookAt * speedFactor;
#ifdef DEBUG_PRINT
		DebugPrint();
#endif
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		CameraPosition -= glm::normalize(glm::cross(LookAt, CameraUpwardVector)) * speedFactor;
#ifdef DEBUG_PRINT
		DebugPrint();
#endif
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		CameraPosition += glm::normalize(glm::cross(LookAt, CameraUpwardVector)) * speedFactor;
#ifdef DEBUG_PRINT
		DebugPrint();
#endif
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		CameraPosition += CameraUpwardVector * speedFactor;
#ifdef DEBUG_PRINT
		DebugPrint();
#endif
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		CameraPosition -= CameraUpwardVector * speedFactor;
#ifdef DEBUG_PRINT
		DebugPrint();
#endif
	}
}

void Camera::ProcessMouse(float xOffset, float yOffset)
{
	if (!useMouse) return;
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	// calculate the mouse position offset then normalize it with the sensitivity
	Yaw += xOffset;
	Pitch += yOffset;

	if(Pitch > 89.0f)
	  Pitch =  89.0f;
	if(Pitch < -89.0f)
	  Pitch = -89.0f;

	UpdateVectors();
}

void Camera::UpdateVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	direction.y = sin(glm::radians(Pitch));
	direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	LookAt = glm::normalize(direction);

#ifdef DEBUG_PRINT
		DebugPrint();
#endif

}
