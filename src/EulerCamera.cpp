#include "EulerCamera.h"

#include <algorithm>

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

EulerCamera::EulerCamera(glm::vec3 position, float fov, float acpectRatio, float nearPlane, float farPlane)
	: Camera(position, fov, acpectRatio, nearPlane, farPlane), m_pitch(0.0f), m_yaw(-90.0f)
{
	m_Up = { .0f, 1.0f, .0f };
	m_front = { 0.0f, 0.0f, -1.0f };
	m_view = glm::lookAt(position, position + m_front, m_Up);
}

void EulerCamera::processKeyBoard(CameraDirection direction, float deltaTime)
{
	float cameraSpeed = defaultCameraSpeed * deltaTime;

	switch (direction)
	{
	case CameraDirection::FORWARD:
		m_position -= cameraSpeed * m_front;
		break;
	case CameraDirection::BACKWARD:
		m_position += cameraSpeed * m_front;
		break;
	case CameraDirection::RIGHT:
		m_position -= cameraSpeed * glm::normalize(glm::cross(m_front, m_Up));
		break;
	case CameraDirection::LEFT:
		m_position += cameraSpeed * glm::normalize(glm::cross(m_front, m_Up));
		break;
	}
	updateViewMatrix();
}
void EulerCamera::processMouseMovement(double offsetX, double offsetY)
{
	offsetX *= defaultSensitivity;
	offsetY *= defaultSensitivity;

	m_yaw += static_cast<float>(offsetX);
	m_pitch += static_cast<float>(offsetY);

	m_pitch = std::clamp(m_pitch, minPitchConstraint, maxPitchConstraint);

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(direction);
	updateViewMatrix();
}
void EulerCamera::processMouseScroll(double offsetY)
{
	m_fov -= static_cast<float>(offsetY);
	m_fov = std::max(m_fov, minFov);
	m_fov = std::min(m_fov, maxFov);
}
void EulerCamera::updateViewMatrix()
{
	m_view = glm::lookAt(m_position, m_position + m_front, m_Up);
}