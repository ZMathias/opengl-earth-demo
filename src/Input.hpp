// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera
{
	// The vector pointing up in world space. Used to calculate the the right vector by cross-multiplying the up vector with the front vector.
	// Cross-multiplying to vectors gives a new vector perpendicular to the "plane" defined by those to vectors
	glm::vec3 CameraUpwardVector{0.0f, 1.0f, 0.0f};

	glm::vec3 CameraRightwardVector{};
	float MouseSensitivity{0.1f};
	float Yaw{-90.0f}, Pitch{0.0f};

public:
	glm::vec3 CameraPosition;
	bool useMouse{true};
	// Initially the vector in the positive Z direction (coming out the screen) used for calculating player position.
	// After moving the camera, the vector is moved to the new direction.
	glm::vec3 LookAt{0.0f, 0.0f, -1.0f};
	bool FirstMouse{true};

	Camera(const glm::vec3& initPos) : CameraPosition(initPos)
	{
		CameraRightwardVector = glm::normalize(glm::cross(LookAt, CameraUpwardVector));
	}

	[[nodiscard]] glm::mat4 RetrieveLookAt() const;

	void ProcessKeyboard(GLFWwindow* window, float deltaTime);
	void ProcessMouse(float xOffset, float yOffset);
	void UpdateVectors();
	void DebugPrint() const
	{
		printf("Camera pos:%f %f %f\n", CameraPosition.x, CameraPosition.y, CameraPosition.z);
		printf("LookAt: %f %f %f\n", LookAt.x, LookAt.y, LookAt.z);
	}
};
