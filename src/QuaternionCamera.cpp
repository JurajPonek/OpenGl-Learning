#include "QuaternionCamera.h"

#include <algorithm>

#include "glm/ext/quaternion_trigonometric.hpp"

QuaternionCamera::QuaternionCamera(glm::vec3 position, glm::vec3 orientation, float fov, float acpectRatio, float nearPlane, float farPlane)
	: Camera(position, fov, acpectRatio, nearPlane, farPlane), m_orientation(glm::vec3(orientation))
{
}

void QuaternionCamera::processKeyBoard(CameraDirection direction, float deltaTime)
{
	float cameraSpeed = defaultCameraSpeed * deltaTime;
	switch (direction)
	{
	case CameraDirection::FORWARD:
		m_position -= cameraSpeed * getFront();
		break;
	case CameraDirection::BACKWARD:
		m_position += cameraSpeed * getFront();
		break;
	case CameraDirection::RIGHT:
		m_position -= cameraSpeed * getRight();
		break;
	case CameraDirection::LEFT:
		m_position += cameraSpeed * getRight();
		break;
	}
	updateViewMatrix();
}
void QuaternionCamera::processMouseMovement(double offsetX, double offsetY)
{
	offsetX *= defaultSensitivity;
	offsetY *= defaultSensitivity;

	glm::quat qPitch = glm::angleAxis(glm::radians(static_cast<float>(offsetY)), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::quat qYaw = glm::angleAxis(glm::radians(-static_cast<float>(offsetX)), glm::vec3(0, 1, 0));

	m_orientation = qYaw * m_orientation * qPitch;

	m_orientation = glm::normalize(m_orientation);
	updateViewMatrix();
}
void QuaternionCamera::processMouseScroll(double offsetY)
{
	m_fov -= static_cast<float>(offsetY);
	m_fov = std::max(m_fov, minFov);
	m_fov = std::min(m_fov, maxFov);
}

void QuaternionCamera::updateViewMatrix()
{
	glm::mat4 rotation = glm::mat4_cast(m_orientation);
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_position);
	m_view = glm::mat4_cast(glm::conjugate(m_orientation)) * translation;
}