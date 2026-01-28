#pragma once
#include "Camera.h"
#include "glm/glm.hpp"

class EulerCamera : public Camera
{
public:
	static constexpr float maxPitchConstraint = 89.0f;
	static constexpr float minPitchConstraint = -89.0f;
public:
	EulerCamera(glm::vec3 position, float fov, float acpectRatio, float nearPlane, float farPlane);

	void processKeyBoard(CameraDirection direction, float deltaTime) override;
	void processMouseMovement(double offsetX, double offsetY) override;
	void processMouseScroll(double offsetY) override;

private:
	void updateViewMatrix() override;

private:
	glm::vec3 m_Up;
	glm::vec3 m_front;
	float m_pitch;
	float m_yaw;
};
